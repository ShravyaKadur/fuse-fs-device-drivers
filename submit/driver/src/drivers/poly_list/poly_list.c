#include <minix/drivers.h>
#include <minix/chardriver.h>
#include <minix/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <minix/ds.h>
#include <string.h>
#include <sys/poly_list.h>

/*
 * Function prototypes for the poly_list driver.
 */
static int poly_list_open(message *m);
static int poly_list_close(message *m);
static struct device * poly_list_prepare(dev_t device);
static int poly_list_transfer(endpoint_t endpt, int opcode, u64_t position,
	iovec_t *iov, unsigned int nr_req, endpoint_t user_endpt, unsigned int
	flags);
static int poly_list_ioctl(message *m);

/* SEF functions and variables. */
static void sef_local_startup(void);
static int sef_cb_init(int type, sef_init_info_t *info);
static int sef_cb_lu_state_save(int);
static int lu_state_restore(void);
//static int yesno;
static int msgCase=QUEUE;

/* Entry points to the poly_list driver. */
static struct chardriver poly_driver_tab =
{
    poly_list_open,
    poly_list_close,
    poly_list_ioctl,
    poly_list_prepare,
    poly_list_transfer,
};

struct qstruct{
	int start, end, size;
	int *array;
	int capacity;
};
struct qstruct q={.size = 0,.start=0,.end=0,.capacity=50};
struct sstruct{

	int top;
	int *array;
	int capacity;
};
struct sstruct s={.top=-1,.capacity=50};


/** Represents the /dev/poly_list device. */
static struct device poly_driver_device;

/** State variable to count the number of times the device has been opened. */
static int open_counter;

static int poly_list_open(message *UNUSED(m))
{
//    yesno = random();
//    printf("Random number generated: %d\n", yesno);
    printf("poly_list_open(). Called %d time(s).\n", ++open_counter);
    return OK;
}

static int poly_list_close(message *UNUSED(m))
{
//	free(q.array);
//	free(s.array);
	q.start=0;
	q.end = 0;
	q.size =0;
	s.top = -1;
	msgCase = QUEUE;
    printf("poly_list_close()\n");
    return OK;
}

static struct device * poly_list_prepare(dev_t UNUSED(dev))
{
//	printf("Entered poly_list_prepare\n");
    poly_driver_device.dv_base = make64(0, 0);
    poly_driver_device.dv_size = make64(WRITESIZE, 0);
    return &poly_driver_device;
}

static int poly_list_ioctl(message *m_ptr){
    printf("Entered poly_list_ioctl()\n");
	if ((q.size !=0) || s.top!=-1){
		printf("Cannot change: array not empty\n");
		return 0;
	}
    switch(m_ptr->COUNT) {
        case QUEUE:
            msgCase = QUEUE;
            printf("Selecting queue\n");
            break;

        case STACK:
            msgCase = STACK;
            printf("Selecting stack\n");
            break;

        default:
            msgCase = QUEUE;
            printf("default selection is queue\n");
            break;
    }
    return 0;
}

static int poly_list_transfer(endpoint_t endpt, int opcode, u64_t position,
    iovec_t *iov, unsigned nr_req, endpoint_t UNUSED(user_endpt),
    unsigned int UNUSED(flags))
{
	printf("Entered poly_list_transfer\n");
    int bytes=0, ret;
    char result[WRITESIZE];

    switch(msgCase) {
        case QUEUE:
        	if (q.size==0){
        		q.array = (int*) malloc(q.capacity * sizeof(int));
        	}

        	break;
        case STACK:
        	if (s.top==-1)
        		s.array = (int*) malloc(s.capacity * sizeof(int));
            break;
        default:
            msgCase = QUEUE;
        	if (q.size==0){
        		q.array = (int*) malloc(q.capacity * sizeof(int));
        	}
        	break;
    }

    if (nr_req != 1)
    {
        /* This should never trigger for character drivers at the moment. */
        printf("poly_list: vectored transfer request, using first element only\n");
    }


    int val= 0;
//    char *result="";
    switch (opcode)
    {
        case DEV_GATHER_S:
//        	printf("DEV_GATHER_S: Read\n");
        	if (msgCase==QUEUE){
        		if (q.size == 0){
        			printf("Cant dequeue: array empty!\n");
        			ret = -1;
        			q.start=0;
        			q.end=0;
        			q.size=0;
        			break;
        		}
        		val = q.array[q.start];
        		q.start++;
        		q.size--;

        		sprintf(result,"%d\n",val);
//        		result[WRITESIZE-1]='\0';

				printf("result of DEV_GATHER queue %s\n", result);
			    bytes = WRITESIZE - ex64lo(position) < iov->iov_size ?
			            WRITESIZE - ex64lo(position) : iov->iov_size;

			    if (bytes <= 0)
			    {
			        return OK;
			    }

                ret = sys_safecopyto(endpt, (cp_grant_id_t) iov->iov_addr, 0,(vir_bytes) (result + ex64lo(position)),bytes);
//                iov->iov_size -= bytes;
//                printf("iov %lu\n",iov->iov_size );
        	}

        	else{
        		if (s.top==-1){
        			printf("Cant pop: array empty!\n");
        			ret=-1;
        			break;
        		}
        		val = s.array[s.top];
        		s.top--;

        		sprintf(result,"%d\n",val);
        		result[WRITESIZE-1]='\0';

				printf("result of DEV_GATHER stack %s\n", result);

			    bytes = strlen(result) - ex64lo(position) < iov->iov_size ?
			            strlen(result) - ex64lo(position) : iov->iov_size;
			    if (bytes <= 0)
			    {
			        return OK;
			    }

                ret = sys_safecopyto(endpt, (cp_grant_id_t) iov->iov_addr, 0,
                                    (vir_bytes) (result + ex64lo(position)),
                                     bytes);
//                iov->iov_size -= bytes;

        	}
        	strcpy(result, "\0");
            break;
        case DEV_SCATTER_S:
        	if (msgCase==QUEUE){
				if (q.size==q.capacity){
					printf("Cant enqueue: array full!\n");
					ret = -1;
					break;
				}
				q.size++;

				ret = sys_safecopyfrom(endpt, (cp_grant_id_t) iov->iov_addr, 0,
									(vir_bytes) (result),sizeof(result));
//				printf("q start, end, size  %d %d %d\n", q.start, q.end, q.size);
//	            result[WRITESIZE-1]='\0';
				val = atoi(result);
				printf("val of DEV_SCATTER queue %d\n", val);
				q.array[q.end] = val;
				q.end++;

			}
			else{
				if (s.top==s.capacity-1){
					printf("Cant push: array full!\n");
					ret=-1;
					break;
				}
				ret = sys_safecopyfrom(endpt, (cp_grant_id_t) iov->iov_addr, 0,
									(vir_bytes) (result),sizeof(result));
				val = atoi(result);
				printf("val of DEV_SCATTER stack %d\n", val);
				s.top++;
				s.array[s.top] = val;

			}
        	strcpy(result, "\0");
			break;
        default:
            return EINVAL;
    }
    return ret;
}

static int sef_cb_lu_state_save(int UNUSED(state)) {
/* Save the state. */
    ds_publish_u32("open_counter", open_counter, DSF_OVERWRITE);

    return OK;
}

static int lu_state_restore() {
/* Restore the state. */
    u32_t value;

    ds_retrieve_u32("open_counter", &value);
    ds_delete_u32("open_counter");
    open_counter = (int) value;

    return OK;
}

static void sef_local_startup()
{
    /*
     * Register init callbacks. Use the same function for all event types
     */
    sef_setcb_init_fresh(sef_cb_init);
    sef_setcb_init_lu(sef_cb_init);
    sef_setcb_init_restart(sef_cb_init);

    /*
     * Register live update callbacks.
     */
    /* - Agree to update immediately when LU is requested in a valid state. */
    sef_setcb_lu_prepare(sef_cb_lu_prepare_always_ready);
    /* - Support live update starting from any standard state. */
    sef_setcb_lu_state_isvalid(sef_cb_lu_state_isvalid_standard);
    /* - Register a custom routine to save the state. */
    sef_setcb_lu_state_save(sef_cb_lu_state_save);

    /* Let SEF perform startup. */
    sef_startup();
}

static int sef_cb_init(int type, sef_init_info_t *UNUSED(info))
{
/* Initialize the poly_list driver. */
    int do_announce_driver = TRUE;

    open_counter = 0;
    switch(type) {
        case SEF_INIT_FRESH:
            printf("%s", "I've been started!\n");
        break;

        case SEF_INIT_LU:
            /* Restore the state. */
            lu_state_restore();
            do_announce_driver = FALSE;

            printf("Hey, I'm a new version!\n");
        break;

        case SEF_INIT_RESTART:
            printf("Hey, I've just been restarted!\n");
        break;
    }

    /* Announce we are up when necessary. */
    if (do_announce_driver) {
        chardriver_announce();
    }

    /* Initialization completed successfully. */
    return OK;
}

int main(void)
{
    /*
     * Perform initialization.
     */
    sef_local_startup();

    /*
     * Run the main loop.
     */
    chardriver_task(&poly_driver_tab, CHARDRIVER_SYNC);
    return OK;
}


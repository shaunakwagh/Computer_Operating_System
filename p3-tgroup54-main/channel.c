#include "channel.h"

// Creates a new channel with the provided size and returns it to the caller
// A 0 size indicates an unbuffered channel, whereas a positive size indicates a buffered channel
chan_t* channel_create(size_t size)
{
    /* IMPLEMENT THIS */
    
    chan_t* newchannel = (chan_t*) malloc(sizeof(chan_t));

    if(!newchannel){ //if the channel is not created, return NULL
        return NULL;
    }

    newchannel->buffer =buffer_create(size);
    pthread_mutex_init(&newchannel-> locked, NULL);
    pthread_cond_init(&newchannel-> receivethread, NULL);
     pthread_cond_init(&newchannel-> sendthread, NULL);
    newchannel->closed = false;

    return newchannel;
    
    
}

// Writes data to the given channel
// This can be both a blocking call i.e., the function only returns on a successful completion of send (blocking = true), and
// a non-blocking call i.e., the function simply returns if the channel is full (blocking = false)
// In case of the blocking call when the channel is full, the function waits till the channel has space to write the new data
// Returns SUCCESS for successfully writing data to the channel,
// WOULDBLOCK if the channel is full and the data was not added to the buffer (non-blocking calls only),
// CLOSED_ERROR if the channel is closed, and
// OTHER_ERROR on encountering any other generic error of any sort
enum chan_status channel_send(chan_t* channel, void* data, bool blocking)
{
    /* IMPLEMENT THIS */


     pthread_mutex_lock(&channel-> locked);

    //if the channel is closed
    if (channel-> closed){
        pthread_mutex_unlock(&channel->locked);
        return CLOSED_ERROR;
    }
    while(buffer_capacity(channel->buffer) == buffer_current_size(channel->buffer)){
        
        if(blocking) { 
            pthread_cond_wait(&channel->sendthread, &channel->locked);
        }     
        else{ 
            pthread_mutex_unlock(&channel->locked);
            return WOULDBLOCK;   // WOULDBLOCK if the channel is full and the data was not added to the buffer (non-blocking calls only),
        }
    }
        buffer_add(data, channel->buffer);
        pthread_cond_signal(&channel->receivethread);
        pthread_mutex_unlock(&channel->locked);
        //successfully writing data to the channel
        return SUCCESS;
    
}

// Reads data from the given channel and stores it in the function’s input parameter, data (Note that it is a double pointer).
// This can be both a blocking call i.e., the function only returns on a successful completion of receive (blocking = true), and
// a non-blocking call i.e., the function simply returns if the channel is empty (blocking = false)
// In case of the blocking call when the channel is empty, the function waits till the channel has some data to read
// Returns SUCCESS for successful retrieval of data,
// WOULDBLOCK if the channel is empty and nothing was stored in data (non-blocking calls only),
// CLOSED_ERROR if the channel is closed, and
// OTHER_ERROR on encountering any other generic error of any sort
enum chan_status channel_receive(chan_t* channel, void** data, bool blocking)
{
    /* IMPLEMENT THIS */

    pthread_mutex_lock(&channel-> locked);
    
    //if the channel is closed
    if (channel-> closed){
        pthread_mutex_unlock(&channel->locked);
        return CLOSED_ERROR;
    }
    while(buffer_current_size(channel->buffer) == 0){// finding the size of the buffer
            
        if(blocking){ 
            pthread_cond_wait(&channel->receivethread, &channel->locked);
        }     
        else{         
            pthread_mutex_unlock(&channel->locked);
            return WOULDBLOCK;//if the channel is empty and nothing was stored in data (non-blocking calls only)
        } 
    }
        
    *data = buffer_remove(channel->buffer);
    pthread_cond_signal(&channel->sendthread);
    pthread_mutex_unlock(&channel->locked);
    return SUCCESS;// the data has been received correctly
}

// Closes the channel and informs all the blocking send/receive/select calls to return with CLOSED_ERROR
// Once the channel is closed, send/receive/select operations will cease to function and just return CLOSED_ERROR
// Returns SUCCESS if close is successful,
// CLOSED_ERROR if the channel is already closed, and
// OTHER_ERROR in any other error case
enum chan_status channel_close(chan_t* channel)
{
    /* IMPLEMENT THIS */
    pthread_mutex_lock(&channel-> locked);

    //if the channel is not closed
    if (!channel-> closed ){
        channel->closed = true;
        pthread_cond_broadcast(&channel-> sendthread);
        pthread_cond_broadcast(&channel-> receivethread);
        
    }
    else{
        pthread_mutex_unlock(&channel->locked);
        return CLOSED_ERROR;//if the channel is already closed
    }

    pthread_mutex_unlock(&channel-> locked);
    return SUCCESS;// Returns SUCCESS if close is successful
}


// Frees all the memory allocated to the channel
// The caller is responsible for calling channel_close and waiting for all threads to finish their tasks before calling channel_destroy
// Returns SUCCESS if destroy is successful,
// DESTROY_ERROR if channel_destroy is called on an open channel, and
// OTHER_ERROR in any other error case
enum chan_status channel_destroy(chan_t* channel)
{
    if(channel->closed == false){
        return DESTROY_ERROR; //if channel_destroy is called on an open channel
    }
    // Frees all the memory allocated to the channel
    pthread_cond_destroy(&channel->sendthread);
    pthread_cond_destroy(&channel->receivethread);
    pthread_mutex_destroy(&channel->locked);
   buffer_free(channel->buffer);
    free(channel);
// destroy is successful
    return SUCCESS;
}

// Takes an array of channels, channel_list, of type select_t and the array length, channel_count, as inputs
// This API iterates over the provided list and finds the set of possible channels which can be used to invoke the required operation (send or receive) specified in select_t
// If multiple options are available, it selects the first option and performs its corresponding action
// If no channel is available, the call is blocked and waits till it finds a channel which supports its required operation
// Once an operation has been successfully performed, select should set selected_index to the index of the channel that performed the operation and then return SUCCESS
// In the event that a channel is closed or encounters any error, the error should be propagated and returned through select
// Additionally, selected_index is set to the index of the channel that generated the error
enum chan_status channel_select(size_t channel_count, select_t* channel_list, size_t* selected_index)
{
    //tried implementin select
   /* 

for (size_t y = 0; y < channel_count; y++)
    {
        
        chan_t *channels = channel_list[y].channel;
        if(channel_list[y].is_send)
        {
            
            pthread_mutex_lock (&channels->sendthread);
        }
        else
        {
            pthread_mutex_lock ( &channels->receivethread);
        }
    }*/

    return SUCCESS;
}

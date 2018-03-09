
#ifndef ESE_FLOW_CONSUMER_HXX
#define ESE_FLOW_CONSUMER_HXX

#include <atomic>
#include <chrono>
#include <functional>
#include <ese/flow/receiver.hxx>

namespace ese
{
    namespace flow
    {
        template<typename TElement>
        class Consumer;

        /**
         *  \brief A factory class that create ::Consumer objects, that consumes from a specified ::Receiver object and
         *      with a specified method for consuming them.
         *  \tparam TElement The type of the elements to consume.
         *  \sa Consumer
         *
         *  This is an abstract class and in order to tell how elements are consumed the consume_0() method have to be
         *  implemented.
         * */
        template<typename TElement>
        class ConsumerFactory
        {
        public:
            /**
             * \brief The type of the elements to consume.
             * */
            typedef TElement ElementType;

            /**
             * \brief The type of the receiver from which the consumers will receive elements.
             * */
            typedef Receiver<TElement> ReceiverType;

            /**
             * \brief The type of the created consumers.
             * */
            typedef Consumer<TElement> ConsumerType;

            /**
             * \brief Construct an object that will create ::Consumer objects, that will consume elements from the
             *     specified ::Receiver object.
             * \param receiver The receiver from which the consumers will receive elements.
             * */
            ConsumerFactory(ReceiverType* receiver) noexcept;

            /**
             * \brief Void implementation.
             * */
            virtual ~ConsumerFactory() noexcept;

            /**
             * \brief Create a ::Consumer object that consumes one element at a time.
             * \param blocking If true, the consumer will block until an element is received, if false and there is
             *     no element available, the consumer will return immediately.
             * \return The created ::Consumer object.
             * \sa create_one_until()
             * \sa create_one_for()
             * \sa create_n()
             *
             * If there is an element to consume, that element would be consumed by the consumer and that would return,
             * otherwise, if there is no element to consume, the consumer would return (returning some negative feedback
             * as return value).
             * */
            ConsumerType create_one(bool blocking = false);

            /**
             * \brief Create a ::Consumer object that consumes one element at a time, waiting until a specified time
             *    point.
             * \param time The time point to wait until.
             * \return The created ::Consumer object.
             * \sa create_one()
             * \sa create_one_for()
             * \sa create_n_until()
             *
             * If there is an element to consume, that element would be consumed by the consumer and that would return,
             * otherwise, if there is no element to consume, the consumer would wait until the specified time point for
             * new elements, if there is no new element, the consumer would return (returning some negative feedback
             * as return value).
             * */
            template<class Clock, class Duration>
            ConsumerType create_one_until(const std::chrono::time_point<Clock, Duration>& time);

            /**
             * \brief Create a ::Consumer object that consumes one element at a time, waiting for a specified amount of
             *     time.
             * \param duration The amount of time to wait.
             * \return The created ::Consumer object.
             * \sa create_one()
             * \sa create_one_until()
             * \sa create_n_for()
             *
             * If there is an element to consume, that element would be consumed by the consumer and that would return,
             * otherwise, if there is no element to consume, the consumer would wait for the specified amount of time
             * for new elements, if there is no new element, the consumer would return (returning some negative feedback
             * as return value).
             * */
            template<class Rep, class Period>
            ConsumerType create_one_for(const std::chrono::duration<Rep, Period>& duration);

            /**
             * \brief Create a ::Consumer object that tries to consumes n elements at a time.
             * \param blocking If true, the consumer will block until n elements are received, if false and there is
             *     no n elements available, the consumer will (consume available and then) return immediately.
             * \return The created ::Consumer object.
             * \sa create_one()
             * \sa create_n_until()
             * \sa create_n_for()
             *
             * If there are n element to consume, those elements would be consumed by the consumer and that would
             * return, otherwise, if there are no n elements to consume, the consumer will consume all available
             * elements and then will return (returning the number of consumed elements).
             * */
            ConsumerType create_n(int n, bool blocking = false);

            /**
             * \brief Create a ::Consumer object that tries to consumes n elements at a time, waiting until a specified
             *     time point.
             * \param time The time point to wait until.
             * \return The created ::Consumer object.
             * \sa create_one_until()
             * \sa create_n()
             * \sa create_n_for()
             *
             * If there are n element to consume, those elements would be consumed by the consumer and that would
             * return, otherwise, if there are no n elements to consume, the consumer would wait until the specified
             * time point for new elements, if there are no new elements, the consumer will consume all available
             * elements and then will return (returning the number of consumed elements).
             * */
            template<class Clock, class Duration>
            ConsumerType create_n_until(int n, const std::chrono::time_point<Clock, Duration>& time);

            /**
             * \brief Create a ::Consumer object that tries to consumes n elements at a time, waiting for an amount of
             *     time.
             * \param time The amount of time to wait.
             * \return The created ::Consumer object.
             * \sa create_one()
             * \sa create_n_until()
             * \sa create_n_for()
             *
             * If there are n element to consume, those elements would be consumed by the consumer and that would
             * return, otherwise, if there are no n elements to consume, the consumer would wait for the specified
             * amount of time for new elements, if there are no new elements, the consumer will consume all available
             * elements and then will return (returning the number of consumed elements).
             * */
            template<class Rep, class Period>
            ConsumerType create_n_for(int n, const std::chrono::duration<Rep, Period>& duration);

            /**
             * \brief Consumes the passed element.
             * \param element The element to consume.
             *
             *  This method implementation tells how elements are consumed.
             * */
            virtual void consume_0(TElement&& element) = 0;

        private:
            /**
             * \brief The receiver from which the created consumers will receive elements.
             * */
            ReceiverType* receiver;
        };

        /**
         * \brief A class that consumes elements, using a specified behaviour.
         * \tparam TElement The type of the elements to consume.
         * \sa ConsumerFactory
         * \sa consume()
         *
         * Instances of this class are created via an instance of ::ConsumerFactory object. There exists different
         * methods for creation of ::Consumer objects, each will inject a different behaviour to the created consumer.
         * */
        template<typename TElement>
        class Consumer {
        public:
            /**
             * \brief The type of the elements to consume.
             * */
            typedef TElement ElementType;

            /**
             * \brief The type of this class (with templates).
             * */
            typedef Consumer<TElement> ThisType;

            /**
             * \brief The type of factory that is creating objects of this class.
             * */
            typedef ConsumerFactory<TElement> FactoryType;

            /**
             * \brief The type of the receiver from which this class is receiving elements.
             * */
            typedef Receiver<TElement> ReceiverType;

            /**
             * \brief Moves the object.
             * \param other The object to move.
             * */
            Consumer(Consumer<ElementType>&& other);

            /**
             * \brief Delete the inner data.
             * */
            virtual ~Consumer();

            /**
             * \brief Consumes the elements using the specified behaviour.
             * \return The number of consumed elements.
             * */
            int consume();

            /**
             * \brief Consumes the elements using the specified behaviour.
             * \return The number of consumed elements.
             *
             * Simply performs a call to the consume() method.
             * */
            int operator()();

            /**
             * \brief Return the number of overall (via multiple call of consume() method) consumed elements.
             * \return The number of overall consumed elements.
             * */
            int get_consumed_count() const noexcept;

            /**
             * \brief Requires the stop of the consumer.
             * */
            void require_stop() noexcept;

        private:
            /**
             * \brief Structure that stores the inner data of ::Consumer objects.
             * */
            typedef struct _InnerData_
            {
                /**
                 * \brief The behaviour of the consumer.
                 * */
                std::function<int(ThisType*)> behaviour;

                /**
                 * \brief Overall consumed elements count.
                 * */
                int consumed_count;

                /**
                 * \brief Tells if a stop was required.
                 * \sa Consumer::require_stop()
                 * */
                std::atomic_bool stop_required;

                /**
                 * \brief Create the inner data structure with specified consumer behaviour.
                 * \param behaviour The consumer behaviour.
                 * */
                _InnerData_(std::function<int(ThisType*)>&& behaviour);
            }
            InnerData;

            /**
             * \brief Tells if the stop is required.
             *
             * It also resets the "stop_required" variable.
             * */
            bool is_stop_required() noexcept;

            /**
             * \brief The inner data.
             * */
            InnerData* data;

            /**
             * \brief Construct a consumer with the specified behaviour.
             * */
            Consumer(std::function<int(ThisType*)>&& behaviour);

            friend FactoryType;
        };
    }
}

#include "template/consumer.txx"

#endif

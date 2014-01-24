#
# generateDelegate.pl
#
# Copyright (c) 2012-2013, TheLib Team (http://www.thelib.org/license)
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# - Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
# - Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
# - Neither the name of TheLib, TheLib Team, nor the names of its
#   contributors may be used to endorse or promote products derived from this
#   software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THELIB TEAM AS IS AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL THELIB TEAM BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#
# implementDelegate.pl
#
# Copyright (C) 2011 by VISUS (Universitaet Stuttgart).
# Alle Rechte vorbehalten.
#
use strict;


#
# Setup:
##############################################################################
my $delegate_output_file = "../thelib++/include/the/delegate.h";
my $multicast_delegate_output_file = "../thelib++/include/the/multicast_delegate.h";
my $max_param_count = 10;


#
# Functions
##############################################################################

#
# WriteDelegateClass
#
sub WriteDelegateClass {
    # parameters
    my $out = shift;		# output stream
    my $paramCnt = shift;	# number template parameters (excluding the return value)
    my $pCnt = shift;		# number of unspecified template parameters

    # class header
    if ($paramCnt eq $pCnt) {
		# all template parameters are unspecified == fully generic template
        print $out "\n    template<class Rv = void";
        for (my $i = 1; $i <= $paramCnt; $i++) {
            print $out ", class P$i = void";
        }
        print $out ">\n    class delegate {";
    } else {
		# template specialization
        print $out "\n    template<";
        if ($pCnt >= 0) {
            print $out "class Rv";
            for (my $i = 1; $i <= $pCnt; $i++) {
                print $out ", class P$i";
            }
        }
        print $out ">\n    class delegate<";
        if ($pCnt >= 0) {
            print $out "Rv";
        } else {
            print $out "void";
        }
        for (my $i = 1; $i <= $pCnt; $i++) {
            print $out ", P$i";
        }
        for (my $i = $pCnt; $i < $paramCnt; $i++) {
            if ($i >= 0) {
                print $out ", void";
            }
        }
        print $out "> {";
    }

    # text strings
    my $paramList = "";
    my $paramTypeList = "";
    my $paramNameList = "";
    my $returnType = ($pCnt >= 0) ? "Rv" : "void";
    my $returnStatement = ($pCnt >= 0) ? "return " : "";
    if ($pCnt > 0) {
        for (my $i = 1; $i <= $pCnt; $i++) {
            if ($i > 1) {
                $paramList .= ", ";
                $paramTypeList .= ", ";
                $paramNameList .= ", ";
            }
            $paramList .= "P$i p$i";
            $paramTypeList .= "P$i";
            $paramNameList .= "p$i";
        }
    } else {
        $paramList = "void";
        $paramTypeList = "void";
        $paramNameList = "";
    }
    my $paramCtxtTypeList = $paramTypeList;
    my $paramCtxtNameList = $paramNameList;
    if ($paramCtxtTypeList eq "void") {
        $paramCtxtTypeList = "CT1";
        $paramCtxtNameList = "this->ctxt";
    } else {
        $paramCtxtTypeList .= ", CT1";
        $paramCtxtNameList .= ", this->ctxt";
    }

    # class implementation
    print $out qq§
    public:

        /**
         * Ctor
         */
        delegate(void) : callee(nullptr) {
            THE_STACK_TRACE;
            // intentionally empty
        }

        /**
         * Copy ctor
         *
         * \@param src The object to clone from
         */
        delegate(const delegate& src) : callee(nullptr) {
            THE_STACK_TRACE;
            (*this) = src;
        }

        /**
         * Move ctor
         *
         * \@param src The object to move from
         */
        delegate(delegate&& src) : callee(nullptr) {
            THE_STACK_TRACE;
            this->callee.swap(src.callee);
        }

        /**
         * Ctor
         *
         * \@param funcPtr Function pointer to be set
         */
        delegate($returnType (*funcPtr)($paramTypeList))
                : callee((funcPtr == nullptr)
                    ? nullptr
                    : new function_callee(funcPtr)) {
            THE_STACK_TRACE;
            // intentionally empty
        }

        /**
         * Ctor
         *
         * \@param funcPtr Function pointer to be set
         * \@param ctxt The user data context used when calling the function
         */
        template<class CT1, class CT2>
        delegate($returnType (*funcPtr)($paramCtxtTypeList), CT2 ctxt)
                : callee((funcPtr == nullptr)
                    ? nullptr
                    : new function_context_callee<CT1>(funcPtr, ctxt)) {
            THE_STACK_TRACE;
            // intentionally empty
        }

        /**
         * Ctor
         *
         * \@param obj The object to call the member of
         * \@param methPtr The method class pointer to be set (must not be NULL)
         */
        template<class C>
        delegate(C& obj, $returnType (C::*methPtr)($paramTypeList))
                : callee(new method_callee<C>(obj, methPtr)) {
            THE_STACK_TRACE;
            // intentionally empty
        }

        /**
         * Ctor
         *
         * \@param obj The object to call the member of
         * \@param methPtr The method class pointer to be set (must not be NULL)
         * \@param ctxt The user data context used when calling the method
         */
        template<class C, class CT1, class CT2>
        delegate(C& obj, $returnType (C::*methPtr)($paramCtxtTypeList), CT2 ctxt)
                : callee(new method_context_callee<C, CT1>(obj, methPtr, ctxt)) {
            THE_STACK_TRACE;
            // intentionally empty
        }

        /**
         * Dtor
         *
         * Note that no memory will be freed (user data context, etc.)
         */
        ~delegate(void) {
            THE_STACK_TRACE;
            this->callee.reset();
        }

        /**
         * Answer whether the target for this delegate is set
         *
         * \@return True if the target for this delegate is set
         */
        inline bool is_target_set(void) const {
            THE_STACK_TRACE;
            return this->callee;
        }

        /**
         * Sets the target for this delegate
         *
         * \@param src The source delegate object
         */
        void set(const delegate& src) {
            THE_STACK_TRACE;
            this->callee = src.callee;
        }

        /**
         * Unsets the target for this delegate
         *
         * Note that no memory will be freed (user data context, etc.)
         */
        void unset(void) {
            THE_STACK_TRACE;
            this->callee.reset();
        }

        /**
         * Calls the delegate's target
         *
         * Parameter and return value depend on the template arguments of the delegate
         */
        $returnType operator()($paramList) {
            THE_STACK_TRACE;
            std::shared_ptr<abstract_callee> c = this->callee;
            if (!c) {
                throw the::invalid_operation_exception("Delegate target not set", __FILE__, __LINE__);
            }
            ${returnStatement}c->call($paramNameList);
        }

        /**
         * Test for equality
         *
         * \@param rhs The right hand side operand
         *
         * \@return True if this and rhs are equal
         */
        bool operator==(const delegate& rhs) const {
            THE_STACK_TRACE;
            if (!this->callee) {
                return !rhs.callee;
            }
            return this->callee->equals(*rhs.callee);
        }

        /**
         * Assignment operator
         *
         * \@param rhs The right hand side operand
         *
         * \@return A reference to this object
         */
        delegate& operator=(const delegate& rhs) {
            THE_STACK_TRACE;
            this->callee = rhs.callee;
            return *this;
        }

        /**
         * Move operator
         *
         * \@param rhs The right hand side operand
         *
         * \@return A reference to this object
         */
        delegate& operator=(delegate&& rhs) {
            THE_STACK_TRACE;
            this->callee.swap(rhs.callee);
            return *this;
        }

    private:

        /**
         * abstract base class for callee implementations
         */
        class abstract_callee {
        public:

            /** Ctor */
            abstract_callee(void) {
                THE_STACK_TRACE;
                // intentionally empty
            }

            /** Dtor */
            virtual ~abstract_callee(void) {
                THE_STACK_TRACE;
                // intentionally empty
            }

            /** Call */
            virtual $returnType call($paramList) = 0;

            /**
             * Test for equality
             *
             * \@param rhs The right hand side operand
             *
             * \@return True if this and rhs are equal
             */
            virtual bool equals(const abstract_callee& rhs) = 0;

        };

        /**
         * Callee implementation for functions
         */
        class function_callee : public abstract_callee {
        public:

            /**
             * Ctor
             *
             * \@param func The function pointer (must not be NULL)
             */
            function_callee($returnType (*func)($paramTypeList)) : abstract_callee(), func(func) {
                THE_ASSERT(this->func != nullptr);
                THE_STACK_TRACE;
            }

            /** Dtor */
            virtual ~function_callee(void) {
                THE_STACK_TRACE;
                this->func = nullptr; // DO NOT DELETE
            }

            /** Call */
            virtual $returnType call($paramList) {
                THE_STACK_TRACE;
                ${returnStatement}this->func($paramNameList);
            }

            /**
             * Test for equality
             *
             * \@param rhs The right hand side operand
             *
             * \@return True if this and rhs are equal
             */
            virtual bool equals(const abstract_callee& rhs) {
                THE_STACK_TRACE;
                const function_callee *r= dynamic_cast<const function_callee*>(&rhs);
                return (r != nullptr)
                    && (r->func == this->func);
            }

        private:

            /** The function pointer */
            $returnType (*func)($paramTypeList);

        };

        /**
         * Callee implementation for functions with user data context
         */
        template<class CT1>
        class function_context_callee : public abstract_callee {
        public:

            /**
             * Ctor
             *
             * \@param func The function pointer (must not be NULL)
             * \@param ctxt The user data context
             */
            function_context_callee($returnType (*func)($paramCtxtTypeList), CT1 ctxt) : abstract_callee(), func(func), ctxt(ctxt) {
                THE_ASSERT(this->func != nullptr);
                THE_STACK_TRACE;
            }

            /** Dtor */
            virtual ~function_context_callee(void) {
                THE_STACK_TRACE;
                this->func = nullptr; // DO NOT DELETE
            }

            /** Call */
            virtual $returnType call($paramList) {
                THE_STACK_TRACE;
                ${returnStatement}this->func($paramCtxtNameList);
            }

            /**
             * Test for equality
             *
             * \@param rhs The right hand side operand
             *
             * \@return True if this and rhs are equal
             */
            virtual bool equals(const abstract_callee& rhs) {
                THE_STACK_TRACE;
                const function_context_callee *r= dynamic_cast<const function_context_callee*>(&rhs);
                return (r != nullptr)
                    && (r->func == this->func)
                    && (r->ctxt == this->ctxt);
            }

        private:

            /** The function pointer */
            $returnType (*func)($paramCtxtTypeList);

            /** The user data context */
            CT1 ctxt;

        };

        /**
         * Callee implementation for methods
         */
        template<class C>
        class method_callee : public abstract_callee {
        public:

            /**
             * Ctor
             *
             * \@param func The method pointer (must not be NULL)
             * \@param ctxt The user data context
             */
            method_callee(C& obj, $returnType (C::*meth)($paramTypeList)) : abstract_callee(), obj(obj), meth(meth) {
                THE_ASSERT(this->meth != nullptr);
                THE_STACK_TRACE;
            }

            /** Dtor */
            virtual ~method_callee(void) {
                THE_STACK_TRACE;
                this->meth = nullptr; // DO NOT DELETE
            }

            /** Call */
            virtual $returnType call($paramList) {
                THE_STACK_TRACE;
                ${returnStatement}(this->obj.*this->meth)($paramNameList);
            }

            /**
             * Test for equality
             *
             * \@param rhs The right hand side operand
             *
             * \@return True if this and rhs are equal
             */
            virtual bool equals(const abstract_callee& rhs) {
                THE_STACK_TRACE;
                const method_callee *r= dynamic_cast<const method_callee*>(&rhs);
                return (r != nullptr)
                    && (&r->obj == &this->obj)
                    && (r->meth == this->meth);
            }

        private:

            /** The object */
            C& obj;

            /** The method pointer */
            $returnType (C::*meth)($paramTypeList);

        };

        /**
         * Callee implementation for methods with user data context
         */
        template<class C, class CT1>
        class method_context_callee : public abstract_callee {
        public:

            /**
             * Ctor
             *
             * \@param obj The object
             * \@param func The method pointer (must not be NULL)
             * \@param ctxt The user data context
             */
            method_context_callee(C& obj, $returnType (C::*meth)($paramCtxtTypeList), CT1 ctxt) : abstract_callee(), obj(obj), meth(meth), ctxt(ctxt) {
                THE_ASSERT(this->meth != nullptr);
                THE_STACK_TRACE;
            }

            /** Dtor */
            virtual ~method_context_callee(void) {
                THE_STACK_TRACE;
                this->meth = nullptr; // DO NOT DELETE
            }

            /** Call */
            virtual $returnType call($paramList) {
                THE_STACK_TRACE;
                ${returnStatement}(this->obj.*this->meth)($paramCtxtNameList);
            }

            /**
             * Test for equality
             *
             * \@param rhs The right hand side operand
             *
             * \@return True if this and rhs are equal
             */
            virtual bool equals(const abstract_callee& rhs) {
                THE_STACK_TRACE;
                const method_context_callee *r= dynamic_cast<const method_context_callee*>(&rhs);
                return (r != nullptr)
                    && (&r->obj == &this->obj)
                    && (r->meth == this->meth)
                    && (r->ctxt == this->ctxt);
            }

        private:

            /** The object */
            C& obj;

            /** The method pointer */
            $returnType (C::*meth)($paramCtxtTypeList);

            /** The user data context */
            CT1 ctxt;

        };

        /** The callee target */
        std::shared_ptr<abstract_callee> callee;

§;

    # class footer
    print $out "    };\n\n";
}


#
# WriteMulticastDelegateClass
#
sub WriteMulticastDelegateClass {
    # parameters
    my $out = shift;		# output stream
    my $paramCnt = shift;	# number template parameters (excluding the return value)
    my $pCnt = shift;		# number of unspecified template parameters

    # class header
    if ($paramCnt eq $pCnt) {
		# all template parameters are unspecified == fully generic template
        print $out "\n    template<";
        for (my $i = 1; $i <= $paramCnt; $i++) {
			if ($i > 1) {
				print $out ", ";
			}
            print $out "class P$i = void";
        }
       print $out ">\n    class multicast_delegate {";
   } else {
		# template specialization
        print $out "\n    template<";
        for (my $i = 1; $i <= $pCnt; $i++) {
			if ($i > 1) {
				print $out ", ";
			}
            print $out "class P$i";
        }
        print $out ">\n    class multicast_delegate<";
        for (my $i = 1; $i <= $pCnt; $i++) {
			if ($i > 1) {
				print $out ", ";
			}
            print $out "P$i";
        }
        for (my $i = $pCnt; $i < $paramCnt; $i++) {
			if ($i >= 1) {
				print $out ", ";
			}
			print $out "void";
        }
        print $out "> {";
   }

    # text strings
    my $paramList = "";
    my $paramTypeList = "";
    my $paramNameList = "";
    if ($pCnt > 0) {
        for (my $i = 1; $i <= $pCnt; $i++) {
            if ($i > 1) {
                $paramList .= ", ";
                $paramTypeList .= ", ";
                $paramNameList .= ", ";
            }
            $paramList .= "P$i p$i";
            $paramTypeList .= "P$i";
            $paramNameList .= "p$i";
        }
    } else {
        $paramList = "void";
        $paramTypeList = "void";
        $paramNameList = "";
    }

    # class implementation
    print $out qq§
    public:

        /** type of the single delegates stored within this multicast delegate */
        typedef delegate<void, $paramTypeList> single_delegate_type;

        /**
         * Ctor. Creates a delegate with no invocation targets.
         */
        multicast_delegate(void) : targets() {
            THE_STACK_TRACE;
            // intentionally empty
        }

        /** Dtor */
        virtual ~multicast_delegate(void) {
            THE_STACK_TRACE;
            // intentionally empty
        }

        /**
         * Adds a new entry to the list of invocation targets
         *
         * \@param d The new entry to be added to the list of invocation
         *          targets
         */
        void add(const single_delegate_type& d) {
            THE_STACK_TRACE;
            targets.add(d);
        }

        /**
         * Removes an entry from the list of invocation targets
         *
         * \@remarks 'd' must be equal to the item added before in that respect
         *              that the attributes of the single delegate must be
         *              equal, i.e. the function pointer and any object
         *              pointer or context pointer used. The single delegate
         *              object itself does not need to be the same one.
         *
         * \@param d The entry to be removed from the list of invocation
         *          targets
         */
        void remove(const single_delegate_type& d) {
            THE_STACK_TRACE;
            targets.remove(d, true);
        }

        /**
         * Invokes the multicast_delegate and calls all entries in the
         * invocation target list
         */
        void operator()($paramList) {
            THE_STACK_TRACE;

            // The called delegate is be able to remove itself from the
            // list of targets!
            collections::array<single_delegate_type> t(this->targets); // deep copy!
            // This operation is not great, but acceptable as we can
            // assume that not that many delegates will be in this list.

            // TODO: Make Thread safe!

            for (size_t i = 0; i < t.size(); i++) {
                t[i]($paramNameList);
            }
        }

        /**
         * Test for equality
         *
         * \@param rhs The right hand side operand
         *
         * \@return true if rhs and this are equal, false otherwise
         */
        bool operator==(const multicast_delegate& rhs) const {
            THE_STACK_TRACE;
            // This is not entirely correct, because both arrays could contain
            // the same elements but in different order. However, this
            // comparison should be sufficient for must cases.
            return this->targets.equals(rhs.targets);
        }

        /**
         * Adds a new entry to the list of invocation targets
         *
         * \@param d The new entry to be added to the list of invocation
         *          targets
         */
        void operator+=(const single_delegate_type& d) {
            THE_STACK_TRACE;
            this->add(d);
        }

        /**
         * Removes an entry from the list of invocation targets
         *
         * \@remarks 'd' must be equal to the item added before in that respect
         *              that the attributes of the single delegate must be
         *              equal, i.e. the function pointer and any object
         *              pointer or context pointer used. The single delegate
         *              object itself does not need to be the same one.
         *
         * \@param d The entry to be removed from the list of invocation
         *          targets
         */
        void operator-=(const single_delegate_type& d) {
            THE_STACK_TRACE;
            this->remove(d);
        }

    private:

        /** The list of invocation targets */
        collections::array<single_delegate_type> targets;

§;

    # class footer
    print $out "    };\n\n";
}


#
# WriteFileHeader
#
sub WriteFileHeader {
    # parameters
    my $out = shift;
    my $name = shift;
	my $date = shift;

    # class header
	print $out qq§/*
 * the/${name}.h
 *
 * Copyright (c) ${date}, TheLib Team (http://www.thelib.org/license)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of TheLib, TheLib Team, nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THELIB TEAM AS IS AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THELIB TEAM BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
§;
}


#
# WriteDelegateFile
#
sub WriteDelegateFile {
	my $outputFile = shift;
	my $paramCnt = shift;

	open (my $out, ">$outputFile") || die "Cannot write output file \"$outputFile\"\n";

	WriteFileHeader($out, "delegate", "2012-2013");
	print $out qq§/*
 * Delegate.h
 *
 * Copyright (C) 2006 - 2011 by Visualisierungsinstitut Universitaet Stuttgart. 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_DELEGATE_H_INCLUDED
#define THE_DELEGATE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/assert.h"
#include "the/invalid_operation_exception.h"
#include "the/memory.h"
#include "the/stack_trace.h"
#include <memory.h>


/*
 * This file is generated by 'generateDelegate.pl'
 */


namespace the {

§;
	# full class
	print $out qq§
    /**
     * Class representing a single callback target, which can either be a
     * function (c or static member) or a member-object pair, both with an
     * optional context object (usually a pointer).
     *
     * The first template parameter specifies the return value.
     * The other template parameters define the parameter list of the
     * callback, while void is used to (implicitly) terminate the list.
     * The parameter list may only have a maximum of $paramCnt elements.
     */§;
	WriteDelegateClass($out, $paramCnt, $paramCnt);

	# specialization classes for smaller parameter lists
	for (my $pcnt = $paramCnt - 1; $pcnt >= 0; $pcnt--) {
		print $out qq§
    /**
     * Template specialication for delegate functions with $pcnt parameters
     */§;
		WriteDelegateClass($out, $paramCnt, $pcnt);
	}

	# specialization class for empty parameter lists
	print $out qq§
    /**
     * Template specialication for delegate functions without parameters and
     * without return value.
     */§;
	WriteDelegateClass($out, $paramCnt, -1);

	# footer
	print $out qq§} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_DELEGATE_H_INCLUDED */
§;
	close ($out);
}


#
# WriteMulticastDelegateFile
#
sub WriteMulticastDelegateFile {
	my $outputFile = shift;
	my $paramCnt = shift;

	open (my $out, ">$outputFile") || die "Cannot write output file \"$outputFile\"\n";

	WriteFileHeader($out, "multicast_delegate", "2013");
	print $out qq§

#ifndef THE_MULTICAST_DELEGATE_H_INCLUDED
#define THE_MULTICAST_DELEGATE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/assert.h"
#include "the/collections/array.h"
#include "the/delegate.h"
#include "the/invalid_operation_exception.h"
#include "the/memory.h"
#include "the/not_copyable.h"
#include "the/stack_trace.h"


/*
 * This file is generated by 'generateDelegate.pl'
 */


namespace the {

§;

	# full class
	print $out qq§
    /**
     * A multicast delegate is a delegate which holds a list of invocation
     * targets. When the delegate is called, all invocation targets are
     * called. The return value of the invocation methods must be void and is
     * therefore omitted in the template parameter list.
     *
     * The template parameters define the parameter list of the callback,
     * while void is used to (implicitly) terminate the list.
     * The parameter list may only have a maximum of 10 elements.
     */§;
	WriteMulticastDelegateClass($out, $paramCnt, $paramCnt);

	# specialization classes for smaller parameter lists
	for (my $pcnt = $paramCnt - 1; $pcnt >= 0; $pcnt--) {
		print $out qq§
    /**
     * Template specialication for multicast_delegates with $pcnt parameters
     */§;
		WriteMulticastDelegateClass($out, $paramCnt, $pcnt);
	}

	# footer
	print $out qq§} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MULTICAST_DELEGATE_H_INCLUDED */
§;
	close ($out);
}


#my $delegate_output_file = "../thelib++/include/the/delegate.h";
#my $multicast_delegate_output_file = "../thelib++/include/the/multicast_delegate.h"
#my $max_param_count = 10;

WriteDelegateFile($delegate_output_file, $max_param_count);
WriteMulticastDelegateFile($multicast_delegate_output_file, $max_param_count);

exit 0;

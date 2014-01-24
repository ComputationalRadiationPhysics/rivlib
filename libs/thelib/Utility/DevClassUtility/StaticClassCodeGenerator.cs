using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DevClassUtility {

    internal class StaticClassCodeGenerator : AbstractCodeGenerator {

        public override string MakeHeaderFile() {
            string year = DateTime.Now.Year.ToString();

            StringBuilder sb = new StringBuilder();
            sb.Append(this.copyrightComment(string.Format("{0}/{1}.h", this.JoinedNamespace("/"), this.ClassName)));
            sb.Append(this.includeGuardStart);
            sb.Append("\n");
            sb.Append(this.mandatoryHeaderIncludes);
            sb.Append("\n\n\n");
            this.addNamespacesOpen(sb);

            sb.Append(string.Format(@"
    /**
     * TODO: Document
     */
    class {0} {{
    public:

    protected:
    private:

        /** forbidden Ctor */
        {0}(void);

        /** forbidden Copy Ctor */
        {0}(const {0}& src);

        /** forbidden Dtor */
        ~{0}(void);

    }};

", this.ClassName));

            this.addNamespacesClose(sb);
            sb.Append(this.includeGuardEnd);
            this.unifyNewLines(ref sb);
            return sb.ToString();
        }

        public override string MakeSourceFile() {
            StringBuilder sb = new StringBuilder();
            sb.Append(this.copyrightComment(string.Format("{0}/{1}.h", this.JoinedNamespace("/"), this.ClassName)));
            sb.Append(string.Format("#include \"{0}/{1}.h\"\n", this.JoinedNamespace("/"), this.ClassName));
            sb.Append(string.Format(@"#include ""the/invalid_operation_exception.h""

/*
 * {1}::{0}::{0}
 */
{1}::{0}::{0}(void) {{
    throw the::invalid_operation_exception(""{0}::ctor"", __FILE__, __LINE__);
}}


/*
 * {1}::{0}::{0}
 */
{1}::{0}::{0}(const {0}& src) {{
    throw the::invalid_operation_exception(""{0}::ctor"", __FILE__, __LINE__);
}}


/*
 * {1}::{0}::~{0}
 */
{1}::{0}::~{0}(void) {{
    throw the::invalid_operation_exception(""{0}::dtor"", __FILE__, __LINE__);
}}
", this.ClassName, this.JoinedNamespace("::")));

            this.unifyNewLines(ref sb);
            return sb.ToString();
        }

    }

}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DevClassUtility {

    internal class NormalClassCodeGenerator : AbstractCodeGenerator {

        public override string MakeHeaderFile() {
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

        /**
         * Ctor
         */
        {0}(void);

        /**
         * Dtor
         */
        ~{0}(void);

    protected:
    private:
    }};

", this.ClassName));

            this.addNamespacesClose(sb);
            sb.Append(this.includeGuardEnd);
            this.unifyNewLines(ref sb);
            return sb.ToString();
        }

        public override string MakeSourceFile() {
            StringBuilder sb = new StringBuilder();
            sb.Append(this.copyrightComment(string.Format("{0}/{1}.cpp", this.JoinedNamespace("/"), this.ClassName)));
            sb.Append(string.Format("#include \"{0}/{1}.h\"\n", this.JoinedNamespace("/"), this.ClassName));
            sb.Append(string.Format(@"

/*
 * {1}::{0}::{0}
 */
{1}::{0}::{0}(void) {{
    /*
     * TODO: Implement
     */
}}


/*
 * {1}::{0}::~{0}
 */
{1}::{0}::~{0}(void) {{
    /*
     * TODO: Implement
     */
}}
", this.ClassName, this.JoinedNamespace("::")));

            this.unifyNewLines(ref sb);
            return sb.ToString();
        }

    }

}

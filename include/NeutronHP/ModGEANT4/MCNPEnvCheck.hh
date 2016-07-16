#ifndef MCNPENVCHECK_HH
#define MCNPENVCHECK_HH


class MCNPEnvCheck
{
    public:
        MCNPEnvCheck();
        virtual ~MCNPEnvCheck();

        static void SetFlag(bool check) { runLikeMCNP = check; }
        static bool GetFlag() { return runLikeMCNP; }

    protected:
    private:

    static bool runLikeMCNP;
};

#endif // MCNPENVCHECK_HH

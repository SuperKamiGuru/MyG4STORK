#ifndef NEUTRONHPMODCHECK_HH
#define NEUTRONHPMODCHECK_HH


class NeutronHPModCheck
{
    public:
        NeutronHPModCheck();
        virtual ~NeutronHPModCheck();

        static void SetFlag(bool check) { useNeutronHPMod = check; }
        static bool GetFlag() { return useNeutronHPMod; }

    protected:
    private:

    static bool useNeutronHPMod;
};

#endif // NEUTRONHPMODCHECK_HH

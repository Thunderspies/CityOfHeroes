#############################################################
## FireShield.fx
#############################################################

FxInfo

#############################################################

Condition
        On              Triggerbits
        Triggerbits     SHIELD
        DoMany          1

	Event
		EName	TalsorianShield
		Type	Local
		At	Hips
                While   SHIELD
                Flags   OneAtATime
		ChildFX	:Child_MagmaShield_Flames.fx
	End
End

#############################################################

End
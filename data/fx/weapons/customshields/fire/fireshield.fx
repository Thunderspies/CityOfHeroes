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
		ChildFX	:Child_FireShield_Flames.fx
	End
End

#############################################################

End
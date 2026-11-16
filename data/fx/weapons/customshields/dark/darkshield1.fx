#############################################################
## DarkShield1.fx
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
		ChildFX	:Child_DarkShield1.fx
	End
End

#########################################################

End
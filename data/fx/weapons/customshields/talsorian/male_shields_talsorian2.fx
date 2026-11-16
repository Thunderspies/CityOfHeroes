#############################################################
## Male_Shields_Talsorian1.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress IsShield

#############################################################

Condition
        On              Triggerbits
        Triggerbits     SHIELD
        DoMany          1

	Event
		EName	TalsorianShield
		Type	Local
		At	LArmL
                While   SHIELD
                Flags   OneAtATime
		ChildFX	WEAPONS\CustomShields\Talsorian\Child_Male_Shields_Talsorian1.fx
	End
End

#############################################################

End
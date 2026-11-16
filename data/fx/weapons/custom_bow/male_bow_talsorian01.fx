#############################################################
## Costume Anim Entity - Bow Base
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Anim	CustomWeapon_Bow_Talsorian_Grip01
	End

	Event
		Type	Local
		At	WepL
		Anim	CustomWeapon_Bow_Talsorian_Arms01
	End
End

#############################################################

Condition
        On		Triggerbits
        Triggerbits	LEFTHAND
        DoMany		1

	Event
		EName	TalsorianBlades_Glow
		Type	Local
		At	WepL
                While   LEFTHAND
                Flags   OneAtATime
		ChildFX	:FX_Bow_Talsorian_01.fx
	End
End


#############################################################

End
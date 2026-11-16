#########################################################
##	template

FxInfo

Flags InheritAlpha InheritAnimScale


Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepL
		Anim	CustomWeapon_Bow_Cupid_01
		BhvrOverride
			tintGeom 1
		End
		Sound Bowout 50 50 .2
	End
End



End
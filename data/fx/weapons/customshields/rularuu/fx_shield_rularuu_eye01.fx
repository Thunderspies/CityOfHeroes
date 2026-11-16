#########################################################
## FX Geo - Talsorian Blade Right
#########################################################

FxInfo

Flags InheritAlpha DontSuppress IsShield

#########################################################

Condition
        On	Time
        Time	0

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PyrRotate	0 0 180
			Scale		1.125 1.125 1.125
		End
		Anim	CustomShield_RularuuShield_Eyelid01
	End
End

#########################################################

End
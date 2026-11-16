#########################################################
## FX Geo - Talsorian Blade Right
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
        On	Time
        Time	0

#	Event
#		Type	Local
#		At	Chest
#		Geom	GEO_RularuuWeapons_Eye01
#	End

	Event
		Type	Local
		At	Waist
		BhvrOverride
			Scale	1.1 1.1 1.1
		End
		Anim	CustomWeapon_RularuuWeapons_Eyelid01
	End

	Event
		Type	Local
		At	Neck
		BhvrOverride
			Scale	1.1 1.1 1.1
		End
		Anim	CustomWeapon_RularuuWeapons_Eyelid01
	End
End

#########################################################

End
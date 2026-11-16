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
#		BhvrOverride
#			Scale		0.875 0.875 0.875
#		End
#		Geom	GEO_RularuuWeapons_Eye01
#	End

	Event
		Type	Local
		At	Waist
		Anim	CustomWeapon_RularuuWeapons_Eyelid01
	End

	Event
		Type	Local
		At	Neck
		Anim	CustomWeapon_RularuuWeapons_Eyelid01
	End
End

#########################################################

End
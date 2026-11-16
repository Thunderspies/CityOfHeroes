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
#			Scale	0.8125 0.8125 0.8125
#		End
#		Geom	GEO_RularuuWeapons_Eye01
#	End

	Event
		Type	Local
		At	Waist
		BhvrOverride
			Scale	0.875 0.875 0.875
		End
		Anim	CustomWeapon_RularuuWeapons_Eyelid01
	End

	Event
		Type	Local
		At	Neck
		BhvrOverride
			Scale	0.875 0.875 0.875
		End
		Anim	CustomWeapon_RularuuWeapons_Eyelid01
	End
End

#########################################################

End
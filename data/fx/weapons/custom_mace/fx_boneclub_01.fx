#########################################################
## FX Geo - Mace 01
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
	On 		Triggerbits
	Triggerbits	RIGHTHAND
	DoMany		1

	Event
		EName	WeaponDrawn
		While	RIGHTHAND
		Flags	OneAtATime
		Type	Local
		At	Origin
		BhvrOverride
			PyrRotate	180 0 0
		End
		Geom	Bone_Club
	End
End

#########################################################

End
#############################################################
## Costume Anim Entity - Assault Rifle - Base
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_BeamRifle\Female_WepRScale.bhvr
		Anim	CustomWeapon_BeamRifle_Shard_Cannon_01
		BhvrOverride
			Scale			0.7 0.7 0.7
			PositionOffset		0.01 0.08 0
			PYRrotate 		0 -6 0
 			PYRrotateJitter 	0 0 0
		End
	End
End

#############################################################

End
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
		Bhvr	WEAPONS\Custom_BeamRifle\Huge_WepRScale.bhvr
		Anim	CustomWeapon_BeamRifle_Shard_Cannon_01
		BhvrOverride
			#Scale			1.5 1.5 1.5
			#PositionOffset		0 0.1 0
			#PYRrotate 		0 -2 -5
# 			PYRrotateJitter 	0 0 0
		End
	End
End

#############################################################

End
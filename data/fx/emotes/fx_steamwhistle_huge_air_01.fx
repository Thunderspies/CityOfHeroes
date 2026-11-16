#############################################################
## FX_CorruptedSeerTentacle_01.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress DontInheritTexFromCostume

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	SteamWhistle
		Type	Local
		At	Root
		Anim	FX_SteamWhistle_01
		BhvrOverride
			Scale			0.02 0.02 0.02
			PositionOffset		-2.6 9.4 1.0
			PYRrotate 		0.0 90.0 0
 			PYRrotateJitter 	0 0 0
		End
	End

End

#############################################################

End
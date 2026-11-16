#############################################################
## Child_SteamJet_Small.fx
#############################################################

FxInfo
Lifespan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	SteamWhistle
		Type	Local
		At	Root
		Anim	FX_SteamWhistle_01
		Bhvr	:WhistleFade.bhvr
		BhvrOverride
			Scale			0.02 0.02 0.02
			PositionOffset		-2.6 9.4 1.0
			PYRrotate 		0.0 90.0 0
 			PYRrotateJitter 	0 0 0
		End
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Ename	WhistleAnchor
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	-2.6 10.2 1.0
			PyrRotate -70 0 0
		End
	End
End

Condition
	On 	Time
	Time	25
	Event
		Type	Local
		At	Root
		Sound 	SP_SteamWhistle_01 100 100 .8
	End
End


Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	WhistleAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0.8 0
		End
		Part	:SteamWhistle_Jet.part
		Lifespan 8
	End

	Event
		Type	Local
		At	WhistleAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0.7 0
		End
		Part	:Steam_Plume_Big.part
		Lifespan 18
	End

	Event
		Type	Local
		At	WhistleAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0.7 0
		End
		Part	:Steam_Plume_BigThick.part
		Lifespan 8
	End
End

Condition
	On 	Time
	Time 	50

	Event
		Type	Local
		At	WhistleAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0.8 0
		End
		Part	:SteamWhistle_Jet.part
		Lifespan 8
	End

	Event
		Type	Local
		At	WhistleAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0.7 0
		End
		Part	:Steam_Plume_Big.part
		Lifespan 18
	End

	Event
		Type	Local
		At	WhistleAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0.7 0
		End
		Part	:Steam_Plume_BigThick.part
		Lifespan 8
	End
End
##############################################################

End
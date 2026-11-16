#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

LifeSpan 250

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	StartPositOnly
		At	Root
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
#		BhvrOverride
#			PositionOffset		0 0.3 0
#			Scale			0.01 0.01 0.01
#			ScaleRate		3.0 3.0 3.0
# 			EndScale		2.5 2.5 2.5
#		End
	#	Part	:FireRing_Small.part
		Part	:Dust_Circle_Expanding_Lingering.part
		Lifespan 60
	End

	Event
		Type	Local
		At	FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 -0.2 0.0
		End
		Part	:Thruster_Smoke.part
		Sound WW_JumpJet_01 200 100 .8
		Lifespan 5
	End

	Event
		Type	Local
		At	FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 -0.2 0.0
		End
		Part	:Thruster_Smoke.part
		Sound WW_JumpJet_02 200 100 .8
		Lifespan 5

	End

	Event
		Type	Local
		At	FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 -0.8 0.0
		End
		Part	:Thruster_Base.part
		Part	:Thruster_Rings.part
	End

	Event
		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset -0.6 -0.5 0.0
		End
		Part	:Thruster_Base.part
		Part	:Thruster_Rings.part
		Part	:Thruster_Core_Small.part
		Part	:Thruster_Tail_Small.part
	End

	Event
		Type	Local
		At	FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 -0.8 0.0
		End
		Part	:Thruster_Base.part
		Part	:Thruster_Rings.part
	End

	Event
		Type	Local
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.6 -0.5 0.0
		End
		Part	:Thruster_Base.part
		Part	:Thruster_Rings.part
		Part	:Thruster_Core_Small.part
		Part	:Thruster_Tail_Small.part
	End

	Event
		Type	Local
		At	FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 -0.2 0.0
		End
		Part	:Thruster_Core.part
		Part	:Thruster_Tail.part

	End

	Event
		Type	Local
		At	FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 -0.2 0.0
		End
		Part	:Thruster_Core.part
		Part	:Thruster_Tail.part
	End

	Event
		Type	Local
		At	FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 -0.4 0.75
		End
		Part	:Thruster_Core_Small.part

	End

	Event
		Type	Local
		At	FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 -0.4 0.75
		End
		Part	:Thruster_Core_Small.part
	End

#############################################################

	Event
		Type	Local
		At	FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 -0.8 0.75
		End
		Part	:Thruster_Base_Small.part
		Sound WW_JumpJetL_Loop 100 100 .5
		bhvr	behaviors/Sound/SoundIn1.bhvr
	End

	Event
		Type	Local
		At	FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 -0.8 0.75
		End
		Part	:Thruster_Base_Small.part
		Sound WW_JumpJetR_Loop 100 100 .5
		bhvr	behaviors/Sound/SoundIn1.bhvr
	End
End
#############################################################
Condition
	On	Time
	Time	5

#	Event
#		Type	Local
#		At	WepL
#		Bhvr	behaviors/GenericParticleFade.bhvr
#		BhvrOverride
#			PositionOffset 0.0 -0.2 0.0
#		End
#		Part	:Thruster_Smoke.part
#		Lifespan 5
#	End
#
#	Event
#		Type	Local
#		At	WepR
#		Bhvr	behaviors/GenericParticleFade.bhvr
#		BhvrOverride
#			PositionOffset 0.0 -0.2 0.0
#		End
#		Part	:Thruster_Smoke.part
#		Lifespan 5
#
#	End

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 -0.8 0.0
		End
		Part	:Thruster_Base.part
		Part	:Thruster_Rings.part
	End

	Event
		Type	Local
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 -0.8 0.0
		End
		Part	:Thruster_Base.part
		Part	:Thruster_Rings.part
	End

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 -0.2 0.0
		End
		Part	:Thruster_Core.part
		Part	:Thruster_Tail.part

	End

	Event
		Type	Local
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 -0.2 0.0
		End
		Part	:Thruster_Core.part
		Part	:Thruster_Tail.part
	End


#############################################################



End

#############################################################

End
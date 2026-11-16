#############################################################
## Protean_PowerSiphon.Fx
#############################################################

FxInfo
LifeSpan	60

#########################################################
####################### AUDIO ###########################
#########################################################



#########################################################
#################### AOE Indicator ########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	BubbleAnchor
		Type	Local
		At	Root
		Sound Protean_SiphonAttack_01 200 200 1
		LifeSpan	30
	End

	Event
		Type	Local
		At	Root
		Bhvr	behaviors/CameraShake01.bhvr
		BhvrOverride
			PositionOffset	0 .1 0
		End
		Part	:Root_CircleShrinking_Fast.part
		Part	:Root_Glow.part

		LifeSpan	40
	End


	Event
		Type	Local
		At	Chest
		Part	:Chest_Shockwave.part
		Part	:Chest_Glow.part
		Part	:Chest_Flash.part
		LifeSpan	30
	End
End

Condition
	On	Cycle
	Time	3

	Event
		Type	Start
		At	BubbleAnchor
		bhvr	V_COV\Powers\EnergyAura\EnergyDrainScale.bhvr
		BhvrOverride
			Alpha		100
			Scale		10.0 10.0 10.0
			EndScale	2.0 2.0 2.0
			StartColor	255 50 0
			PyrRotate 	0 90 0
		End
		Geom	EarthGraspHit
		LifeSpan	5

	End
End

##########################################

End
#############################################################
## EnergyEmote.fx
#############################################################

FxInfo
LifeSpan	90

#############################################################
##################    IMPLOSION   ###########################
#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	EndGame_PulseEnergy 100 100 .8
	End
End


Condition
	On	Time
	Time	0

	Event
		Ename	MainRing
		Type	Start
		At	Chest
		Part	:EnergyBall_Gathering.part
		Part	:EnergyBall_Dark_Gathering.part

		PMagnet	Chest
		Lifespan 40
	End

End

Condition
	On	Time
	Time	10

	Event
		Ename	MainRing
		Type	Start
		At	Chest
		Part	:Glow_Chest.part
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	45

	Event
		Ename	MainRing
		Type	Start
		At	chest
		Part	:Blast_Ring.part
	End

	Event
		Ename	Ring1
		Type	Start
		At	chest
		Part	:EnergyShockRing_Shrink.part
	End

	Event
		Ename	Ring2
		Type	Start
		At	chest
		Bhvr	Behaviors/SpinHit2.bhvr
		BhvrOverride
			SpinJitter	.3 .3 .3
		End
		Part	:EnergyShockRing_Shrink.part
	End

	Event
		Ename	Ring3
		Type	Start
		At	chest
		Bhvr	Behaviors/SpinHit2.bhvr
		BhvrOverride
			SpinJitter	.3 .3 .3
		End
		Part	:Blast_Shrink.part
	End



End

#############################################################
##################    FULL-BODY FX   ########################
#############################################################

Condition
	On 	Time
	Time 	51

	Event
		Ename	BodyGlow
		Type	Local
		At	Root
		ChildFX	:BodyGlow.fx

	End

#############################################################
##################   ORBITING PARTICLES  ####################
#############################################################

	Event
		Ename	Ring1
		Type	Start
		At	chest
		Bhvr	Behaviors/SpinHit1.bhvr
		Sound Megablasthit2 80 80 .95
	End

	Event
		Type	local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Pmagnet	Chest
		Part	:EnergyBall.part
		Part	:DarkEnergyBall.part
		Lifespan	45
		LifespanJitter	5
	End


#############################################################
##################   PHYSICS  ###############################
#############################################################

	Event
		Ename	PHYSICS-ExplosiveForce
		Type	Start
		At	Chest

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		600
			PhysForcePowerJitter	100
		End
		Lifespan	1

	End
End

#############################################################
##################   EXPLOSION  #############################
#############################################################

Condition

	On 	Time
	Time 	53

	Event
		Type	Start
		At	Hips
		Part	:Shockwave.part
		Part	:Blast.part
		Part	:Blast_Large.part
		Part	:EnergyBall_Exploding.part
		Part	:EnergyBall_Dark_Exploding.part
		Part	:Spikes.part
	End

#############################################################
##################   ORBITING RINGS  ########################
#############################################################

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors/SpinHit1.bhvr
		BhvrOverride
			SpinJitter	.06 .06 .06
		End
		Part	:EnergyShockRing.part
		Lifespan 	20
	End


	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors/SpinHit2.bhvr
		BhvrOverride
			SpinJitter	.06 .06 .06
		End
		Part	:EnergyShockRing.part
		Lifespan 	20
	End

	Event
		Type	start
		At	Chest
		Bhvr	Behaviors/SpinHit3.bhvr
		BhvrOverride
			SpinJitter	.06 .06 .06
		End
		Part	:EnergyShockRing.part
		Part	:Shockwave_Upright.part
		Lifespan 	20
	End

	Event
		EName	ring
		Type	start
		At	Chest
		bhvr	:RingScale.bhvr
		geom	MegaRing2
		LifeSpan 1
	End
End

Condition
	On 	Time
	Time 	65
	Repeat	4

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		ChildFX	:CHILD_Electron.fx
		Lifespan 100
	End
End

End




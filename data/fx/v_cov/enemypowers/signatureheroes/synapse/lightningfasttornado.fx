#########################################################
##	FireRing
##
FxInfo


LifeSpan	90

Condition
	On Time 
	Time 0
	
	Event
		Type 	local	
		At	origin
		
		Sound hasten 70 70 .55
	End

End

	
Condition
	On 	Time
	Time 	20


	Event
		EName 	Prime
		Type	local 
		At	Root
		#bhvr	behaviors/slowpsonictornadomotion.bhvr
#		part1	POWERS\SuperSpeed\SpeedGlowLocal02.part	
#		part2	POWERS\SuperSpeed\SpeedEmberLocal.part
#		part3	POWERS\SuperSpeed\SpeedEmber.part
#		Part4	POWERS\SuperSpeed\SpeedCloudBaseGlow.part
#		part5	POWERS\SuperSpeed\SpeedStreaks.part
		Geom	Dummy02	

		Lifespan	60
	End

	Event
		
		Type	Local 
		At	Prime
		 AltPiv 1
#		part1	POWERS\SuperSpeed\SpeedFlame01.part
#		part2	POWERS\SuperSpeed\SpeedFlame01Flat.part
#		part3	POWERS\SuperSpeed\SpeedFire.part
	End

	Event
		
		Type	Local 
		At	Prime
		 AltPiv 2
#		part1	POWERS\SuperSpeed\SpeedFlame01.part
#		part2	POWERS\SuperSpeed\SpeedFlame01Flat.part
#		part3	POWERS\SuperSpeed\SpeedFire.part
	End

	Event
		
		Type	Local 
		At	Prime
		 AltPiv 1
#		#Part1	POWERS\SuperSpeed\SpeedCloudStreaks.part
#		part2	POWERS\SuperSpeed\SpeedFire.part
#		part3	POWERS\SuperSpeed\SpeedGlow.part
		Sound whirly_loop 60 60 .42
		
	End

	Event
		
		Type	Local 
		At	Prime
		 AltPiv 2
#		#Part1	POWERS\SuperSpeed\SpeedCloudStreaks.part
#		part2	POWERS\SuperSpeed\SpeedFire.part
#		part3	POWERS\SuperSpeed\SpeedGlow.part
#		Sound	hasten 80 30 .7
	End

####################################################################################

	Event
		EName 	Ring1
		Type	local 
		At	Prime
	
		BhvrOverride
			HueShift	-150
		End
		
		Part1	POWERS\SuperSpeed\Ensnare.part
		part2	POWERS\SuperSpeed\SpeedStreaks.part	
		#Part2	POWERS\SuperSpeed\Ensnare2.part
		Part3	POWERS\SuperSpeed\EnsnareMist.part
		Part4	POWERS\SuperSpeed\Leaves.part
		Part5	POWERS\SuperSpeed\EnsnareMist2.part
		bhvr	behaviors/SuperSpeedWirlwind.bhvr	
		Sound Whirlwind2_loop 80 80 .8

		Lifespan	80

	End
			
	Event
		Ename	SwirlForce
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 4 0
			PhysForceType		CWSwirl
			PhysForceRadius		15
			PhysForceCentripetal	1.5
			PhysForcePower		250
			PhysForcePowerJitter	5
		End

		Lifespan 90

	End

	Event
		Ename	LevitationForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		12
			PhysForcePower		200
			PhysForcePowerJitter	5
		End

		Lifespan	90

	End

	
	Event
		Ename	SpreadForce
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 7 0
			PhysForceType		Out
			PhysForceRadius		6
			PhysForcePower		30
			PhysForcePowerJitter	5
		End

		Lifespan	90

	End

		
	Event
		Ename	Shhot
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 12 0
			PhysForceType		Out
			PhysForceRadius		5
			PhysForcePower		300
			PhysForcePowerJitter	5
		End

		Lifespan	90

	End
		
	Event
		Ename	Gbase
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 -1 0
			PhysForceType		Out
			PhysForceRadius		5
			PhysForcePower		150
			PhysForcePowerJitter	5
		End

		Lifespan	90

	End

	
End

Condition
	On 	cycle
	Time 	13
	Chance	1

	Event
		EName 	Rings
		Type	Local 
		At	Prime
	
		Bhvr 	behaviors/slowtornadomotion.bhvr
		
		BhvrOverride
			StartColor	175 50 175
		End
		geom	PsionicWisp #SpeedWisp	
		
		Lifespan 45
	End

End

#############  Static Electricity   ##############

Condition
	On 	Time
	Time 	13


	Event
		EName 	electric
		Type	Local 
		At	Root
	
		childFX	POWERS\ElectricityControl\ElectricityArmorEnergy.fx
		
		Lifespan 50
	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName 	electric
		Type	Local 
		At	Root
	
		childFX	POWERS\ElectricityControl\ElectricityFence.fx #ChargedArmor.fx
		
		Lifespan 30
	End

	Event
		
		Type	PositOnly
		At	Hips
		Geom	Cagearchs2
		bhvr	behaviors/cagespin2.bhvr
		BhvrOverride
			Alpha	100
			Scale	.6 1 .6
			FadeInLength	15
			FadeOutLength	30
		End
		Lifespan 50
	End
	
	Event
		
		Type	PositOnly
		At	Hips
		Geom	Cagearchs
		bhvr	behaviors/cagespin.bhvr
		BhvrOverride
			Alpha	100
			Scale	.6 1 .6
			FadeInLength	15
			FadeOutLength	30
		End
		Lifespan 50
	End


End
################################################################
		
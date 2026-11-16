#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	FootR
End

Input  
	InpName	FootL
End

Input  
	InpName	Root
End

Input  
	InpName	Head
End

Input  
	InpName	Chest
End

Input  
	InpName	Hips
End

##################################

#Condition
#	On 	cycle
#	Time 	5
#
#	Event
#		EName 	SplatEmber
#		Type	posit
#
#		At	root
#		
#		Bhvr	:SuperSpeedSplat.bhvr
#		Splat	Ember.tga
#		
#		Lifespan	20	
#	End
#
#End

Condition
	On	Time
	Time	0
	
	Event
		Type	Local 
		At	Root
		Sound ForceField5_loop 40.0 40.0 .6
		bhvr	behaviors/soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 1
	End
	
	Event
		EName 	Prime
		Type	Local 
		At	Root
		part1	:SpeedGlowLocal02.part	
		part2	:SpeedEmberLocal.part
		part	:SpeedEmber.part
		Part	:SpeedCloudBaseGlow.part
		Geom	Dummy02	
		Sound Run5 40.0 40.0 .7
	End

	Event
		Type	Local 
		At	Root
		part	:SpeedCloudBaseGlowFlat.part
		part	:SpeedStreaksLocal.part
		part	:SpeedStreaks.part
#		Sound 	ForceField5_loop 40.0 30.0 .6
#		bhvr	behaviors/soundFade2.bhvr
#		Flags	PowerLoopingSound
	End


	Event
		
		Type	Local 
		At	Prime
		 AltPiv 1
		part1	:SpeedFlame01.part
		part2	:SpeedFlame01Flat.part
		part3	:SpeedFire.part
	End

	Event
		
		Type	Local 
		At	Prime
		 AltPiv 2
		part1	:SpeedFlame01.part
		part2	:SpeedFlame01Flat.part
		part3	:SpeedFire.part
	End

	Event
		
		Type	Local 
		At	Prime
		 AltPiv 1
		Part1	:SpeedCloudStreaks.part
		part2	:SpeedFire.part
		part3	:SpeedGlow.part
		
	End

	Event
		
		Type	Local 
		At	Prime
		 AltPiv 2
		Part1	:SpeedCloudStreaks.part
		part2	:SpeedFire.part
		part3	:SpeedGlow.part
	End


	Event	
		
		Type	Local
		At	FootR
#		part1	:SpeedSpots.part
		PMagnet Head
	End

	Event	
		Type	Local
		At	FootL
		part1	:SpeedSpots.part
		PMagnet Head
	End

#	Event
#		Type	Local
#		At	Hips
#		part1	:SpeedCloud.part
#		
#	End
#
#	Event
#		Type	Local
#		At	Hips
#		part2	:SpeedCloudLocal.part
#		PMagnet   Hips
#	End

	Event
		Ename	LevitationForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		8
			PhysForcePower		200
			PhysForcePowerJitter	15
			PhysForceSpeedScaleMax	60
		End
	End

	Event
		Ename	WakeForce
		Type	Local
		At	Hips
		
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		10
			PhysForcePower		30
			PhysForcePowerJitter	10
			PhysForceSpeedScaleMax	120
		End
	End

	
	Event
		Ename	WakeForce2
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		5
			PhysForcePower		50
			PhysForcePowerJitter	10
			PhysForceSpeedScaleMax	80
		End
	End
End

End				
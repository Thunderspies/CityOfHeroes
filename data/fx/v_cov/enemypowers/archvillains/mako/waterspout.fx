#########################################################
##	FireRing
##
FxInfo

Condition
	On 	Time
	Time 	20

	Event
		EName 	Ring1
		Type	local 
		At	root
	
		Part	:Steam.part
		Part	:WaterDrops.part
		Part	:SteamBright.part
		Part	:WaterDroplets.part

		Part	:Rocks.part
		Part	:Sand.part
		Part	:SandBright.part
		Part	:SandFlat.part
		Part	:SandBrightFlat.part
	
		Part	:SandBrightCore.part

		Part	:WaterDropsCore.part
		Part	:WaterDropsIn.part

		bhvr	behaviors/SuperSpeedWirlwind.bhvr	
		Sound Whirlwind2_loop 80 80 .5

		
	End
		Event
		Ename	SwirlForce1
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 6 0
			PhysForceType		CCWSwirl
			PhysForceRadius		10
			PhysForceCentripetal	.65
			PhysForcePower		100
			PhysForcePowerJitter	5
		End
		Sound WaterFountain2_loop 80 80 1
		#Lifespan 40

	End
			
	Event
		Ename	SwirlForce1
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 16 0
			PhysForceType		CCWSwirl
			PhysForceRadius		12
			PhysForceCentripetal	1
			PhysForcePower		100
			PhysForcePowerJitter	5
		End

		#Lifespan 40

	End

	Event
		Ename	LevitationForce1
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 5 0
			PhysForceType		Up
			PhysForceRadius		12
			PhysForcePower		50
			PhysForcePowerJitter	5
		End

		#Lifespan	30

	End

	
	Event
		Ename	LevitationForce2
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 15 0
			PhysForceType		Up
			PhysForceRadius		10
			PhysForcePower		25
			PhysForcePowerJitter	5
		End

		#Lifespan	30

	End

	
	Event
		Ename	SpreadForce
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 0 0
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		25
			PhysForcePowerJitter	5
		End

		#Lifespan	30

	End

		
	Event
		Ename	Shhot
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 16 0
			PhysForceType		Out
			PhysForceRadius		5
			PhysForcePower		200
			PhysForcePowerJitter	5
		End

		#Lifespan	30

	End

End

Condition
	On 	cycle
	Time 	12

	Event
		EName 	Rings
		Type	Local 
		At	root
		
		Bhvr 	behaviors/WaterSpoutMotion.bhvr
		geom	FX_TmpCtrlWisp01	#PsionicWisp	
		
		Lifespan 20
	End

End

#Condition
#	On 	cycle
#	Time 	12
#
#	Event
#		EName 	Rings
#		Type	Local 
#		At	root
#		
#		Bhvr 	behaviors/WaterSpoutDark.bhvr
#		geom	FX_TmpCtrlWisp01	#PsionicWisp	
#		
#		Lifespan 15
#	End
#
#End
#
#Condition
#	On 	cycle
#	Time 	10
#
#	Event
#		EName 	Rings
#		Type	Local 
#		At	root
#		
#		Bhvr 	behaviors/WaterSpoutMedium.bhvr
#		geom	FX_TmpCtrlWisp01	#PsionicWisp	
#		
#		Lifespan 50
#	End
#
#End

Condition
	On 	Time
	Time 	24

	Event
		Type	local 
		At	root


		
		Bhvr 	behaviors/WaterSpoutCore.bhvr
		geom	WaterFunnel
		
	End

End


End
################################################################
		
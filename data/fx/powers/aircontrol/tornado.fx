#########################################################
##	FireRing
##
FxInfo
	

Input  
	InpName	Root
End

Input  
	InpName	Hips
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Ring
		Type	local 
		At	Root
		Part1	:Ensnare.part
		Part2	:Ensnare2.part
		Part3	:EnsnareMist.part
		Part4	:Leaves.part
		Part5	:EnsnareMist2.part
		bhvr	behaviors/Wirlwind.bhvr	
		Sound tornado_loop 120 120 .84
	End

	
	Event
		Ename	SwirlForce
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 4 0
			PhysForceType		CCWSwirl
			PhysForceRadius		15
			PhysForceCentripetal	.5
			PhysForcePower		150
			PhysForcePowerJitter	5
		End

		#Lifespan 40

	End

	Event
		Ename	LevitationForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		10
			PhysForcePower		150
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
			PhysForceRadius		10
			PhysForcePower		60
			PhysForcePowerJitter	5
		End

		#Lifespan	30

	End

		
	Event
		Ename	Shhot
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 6 0
			PhysForceType		Out
			PhysForceRadius		5
			PhysForcePower		400
			PhysForcePowerJitter	5
		End

		#Lifespan	30

	End

End

Condition
	On 	cycle
	Time 	20
	Chance	1

	Event
		EName 	Rings
		Type	Local 
		At	Root
		
		Bhvr 	behaviors/tornado.bhvr
		geom	FX_AirWisp01	
		Lifespan 140 # From behavior
	End

End



End			
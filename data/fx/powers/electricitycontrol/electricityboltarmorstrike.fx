#########################################################
##	forceBubble
########################################################
FxInfo

######################################################################################


Condition
	On	Cycle
	Time	3
	chance	 .23

	Event

		Ename	TargetNode
		Type	Local
		At	origin
		
		BhvrOverride	
			StartJitter	1.5 1.5 1.5
		End

		LifeSpan	15
		
	End

	Event
		Type	start
		At	WepL
		
		#Part4	:ElectricitySparkBurstSubtle.part
		Part	:ElectricityControl01Subtle.part
		Part	:ElectricityControl02Subtle.part
		Part	:ElectricityInnerGlowSubtle.part
		Part	:ElectricityOuterGlowSubtle.part
		Part	:ElectricityArchSubtle.part
		Part	:ElectricityRingSubtle.part
		Part1	:ChargedBoltA.part
		Part2	:ChargedBoltB.part
		Part3	:ChargedBoltC.part
		Part4	:ChargedBoltD.part
		PMagnet	TargetNode	
		LifeSpan	15
	End	

End

######################################################################################


Condition
	On	Cycle
	Time	4
	chance	 .23

	Event

		Ename	TargetNode1
		Type	Local
		At	origin
		
		BhvrOverride	
			StartJitter	1.5 1.5 1.5
		End

		LifeSpan	15
		
	End

	Event
		Type	start
		At	WepR
		#Part4	:ElectricitySparkBurstSubtle.part
		Part	:ElectricityControl01Subtle.part
		Part	:ElectricityControl02Subtle.part
		Part	:ElectricityInnerGlowSubtle.part
		Part	:ElectricityOuterGlowSubtle.part
		Part	:ElectricityArchSubtle.part
		Part	:ElectricityRingSubtle.part

		Part1	:ChargedBoltA.part
		Part2	:ChargedBoltB.part
		Part3	:ChargedBoltC.part
		Part4	:ChargedBoltD.part
		PMagnet	TargetNode1	
		LifeSpan	15
	End	

End

######################################################################################


Condition
	On	Cycle
	Time	4
	chance	 .23

	Event

		Ename	TargetNode2
		Type	Local
		At	origin
		
		BhvrOverride	
			StartJitter	1.5 1.5 1.5
		End

		LifeSpan	15
		
	End

	Event
		Type	start
		At	FootL
		#Part4	:ElectricitySparkBurstSubtle.part
		Part	:ElectricityControl01Subtle.part
		Part	:ElectricityControl02Subtle.part
		Part	:ElectricityInnerGlowSubtle.part
		Part	:ElectricityOuterGlowSubtle.part
		Part	:ElectricityArchSubtle.part
		Part	:ElectricityRingSubtle.part

		Part1	:ChargedBoltA.part
		Part2	:ChargedBoltB.part
		Part3	:ChargedBoltC.part
		Part4	:ChargedBoltD.part
		PMagnet	TargetNode2	
		LifeSpan	15
	End	

End

######################################################################################


Condition
	On	Cycle
	Time	3
	chance	 .23

	Event

		Ename	TargetNode3
		Type	Local
		At	origin
		
		BhvrOverride	
			StartJitter	1.5 1.5 1.5
		End

		LifeSpan	15
		
	End

	Event
		Type	start
		At	FootR
		#Part4	:ElectricitySparkBurstSubtle.part
		Part	:ElectricityControl01Subtle.part
		Part	:ElectricityControl02Subtle.part
		Part	:ElectricityInnerGlowSubtle.part
		Part	:ElectricityOuterGlowSubtle.part
		Part	:ElectricityArchSubtle.part
		Part	:ElectricityRingSubtle.part

		Part1	:ChargedBoltA.part
		Part2	:ChargedBoltB.part
		Part3	:ChargedBoltC.part
		Part4	:ChargedBoltD.part
		PMagnet	TargetNode3
		LifeSpan	15
	End	

End

######################################################################################


Condition
	On	Cycle
	Time	3
	chance	 .23

	Event

		Ename	TargetNode4
		Type	Local
		At	origin
		
		BhvrOverride	
			StartJitter	1.5 1.5 1.5
		End

		LifeSpan	15
		
	End

	Event
		Type	start
		At	UlegL
		#Part4	:ElectricitySparkBurstSubtle.part
		Part	:ElectricityControl01Subtle.part
		Part	:ElectricityControl02Subtle.part
		Part	:ElectricityInnerGlowSubtle.part
		Part	:ElectricityOuterGlowSubtle.part
		Part	:ElectricityArchSubtle.part
		Part	:ElectricityRingSubtle.part

		Part1	:ChargedBoltA.part
		Part2	:ChargedBoltB.part
		Part3	:ChargedBoltC.part
		Part4	:ChargedBoltD.part
		PMagnet	TargetNode4	
		LifeSpan	15
	End	

End

######################################################################################


Condition
	On	Cycle
	Time	4
	chance	 .23

	Event

		Ename	TargetNode5
		Type	Local
		At	origin
		
		BhvrOverride	
			StartJitter	1.5 1.5 1.5
		End

		LifeSpan	15
		
	End

	Event
		Type	start
		At	ULegR
		#Part4	:ElectricitySparkBurstSubtle.part
		Part	:ElectricityControl01Subtle.part
		Part	:ElectricityControl02Subtle.part
		Part	:ElectricityInnerGlowSubtle.part
		Part	:ElectricityOuterGlowSubtle.part
		Part	:ElectricityArchSubtle.part
		Part	:ElectricityRingSubtle.part

		Part1	:ChargedBoltA.part
		Part2	:ChargedBoltB.part
		Part3	:ChargedBoltC.part
		Part4	:ChargedBoltD.part
		PMagnet	TargetNode5	
		LifeSpan	15
	End	

End

######################################################################################


Condition
	On	Cycle
	Time	4
	chance	 .23

	Event

		Ename	TargetNode6
		Type	Local
		At	origin
		
		BhvrOverride	
			StartJitter	1.5 1.5 1.5
		End

		LifeSpan	15
		
	End

	Event
		Type	start
		At	LlegL
		#Part4	:ElectricitySparkBurstSubtle.part
		Part	:ElectricityControl01Subtle.part
		Part	:ElectricityControl02Subtle.part
		Part	:ElectricityInnerGlowSubtle.part
		Part	:ElectricityOuterGlowSubtle.part
		Part	:ElectricityArchSubtle.part
		Part	:ElectricityRingSubtle.part

		Part1	:ChargedBoltA.part
		Part2	:ChargedBoltB.part
		Part3	:ChargedBoltC.part
		Part4	:ChargedBoltD.part
		PMagnet	TargetNode6	
		LifeSpan	15
	End	

End

######################################################################################


Condition
	On	Cycle
	Time	3
	chance	 .23

	Event

		Ename	TargetNode7
		Type	Local
		At	origin
		
		BhvrOverride	
			StartJitter	1.5 1.5 1.5
		End

		LifeSpan	15
		
	End

	Event
		Type	start
		At	LlegR
		#Part4	:ElectricitySparkBurstSubtle.part
		Part	:ElectricityControl01Subtle.part
		Part	:ElectricityControl02Subtle.part
		Part	:ElectricityInnerGlowSubtle.part
		Part	:ElectricityOuterGlowSubtle.part
		Part	:ElectricityArchSubtle.part
		Part	:ElectricityRingSubtle.part

		Part1	:ChargedBoltA.part
		Part2	:ChargedBoltB.part
		Part3	:ChargedBoltC.part
		Part4	:ChargedBoltD.part
		PMagnet	TargetNode7	
		LifeSpan	15
	End	

End

######################################################################################


Condition
	On	Cycle
	Time	3
	chance	 .23

	Event

		Ename	TargetNode8
		Type	Local
		At	origin
		
		BhvrOverride	
			StartJitter	1.5 1.5 1.5
		End

		LifeSpan	15
		
	End

	Event
		Type	start
		At	UarmL
		#Part4	:ElectricitySparkBurstSubtle.part
		Part	:ElectricityControl01Subtle.part
		Part	:ElectricityControl02Subtle.part
		Part	:ElectricityInnerGlowSubtle.part
		Part	:ElectricityOuterGlowSubtle.part
		Part	:ElectricityArchSubtle.part
		Part	:ElectricityRingSubtle.part

		Part1	:ChargedBoltA.part
		Part2	:ChargedBoltB.part
		Part3	:ChargedBoltC.part
		Part4	:ChargedBoltD.part
		PMagnet	TargetNode8	
		LifeSpan	15
	End	

End

######################################################################################


Condition
	On	Cycle
	Time	4
	chance	 .23

	Event

		Ename	TargetNode9
		Type	Local
		At	origin
		
		BhvrOverride	
			StartJitter	1.5 1.5 1.5
		End

		LifeSpan	15
		
	End

	Event
		Type	start
		At	UarmR
		#Part4	:ElectricitySparkBurstSubtle.part
		Part	:ElectricityControl01Subtle.part
		Part	:ElectricityControl02Subtle.part
		Part	:ElectricityInnerGlowSubtle.part
		Part	:ElectricityOuterGlowSubtle.part
		Part	:ElectricityArchSubtle.part
		Part	:ElectricityRingSubtle.part

		Part1	:ChargedBoltA.part
		Part2	:ChargedBoltB.part
		Part3	:ChargedBoltC.part
		Part4	:ChargedBoltD.part
		PMagnet	TargetNode9	
		LifeSpan	15
	End	

End

######################################################################################


Condition
	On	Cycle
	Time	4
	chance	 .23

	Event

		Ename	TargetNodei
		Type	Local
		At	origin
		
		BhvrOverride	
			StartJitter	1.5 1.5 1.5
		End

		LifeSpan	15
		
	End

	Event
		Type	start
		At	LarmL
		#Part4	:ElectricitySparkBurstSubtle.part
		Part	:ElectricityControl01Subtle.part
		Part	:ElectricityControl02Subtle.part
		Part	:ElectricityInnerGlowSubtle.part
		Part	:ElectricityOuterGlowSubtle.part
		Part	:ElectricityArchSubtle.part
		Part	:ElectricityRingSubtle.part

		Part1	:ChargedBoltA.part
		Part2	:ChargedBoltB.part
		Part3	:ChargedBoltC.part
		Part4	:ChargedBoltD.part
		PMagnet	TargetNodei	
		LifeSpan	15
	End	

End

######################################################################################


Condition
	On	Cycle
	Time	3
	chance	 .23

	Event

		Ename	TargetNodeu
		Type	Local
		At	origin
		
		BhvrOverride	
			StartJitter	1.5 1.5 1.5
		End

		LifeSpan	15
		
	End

	Event
		Type	start
		At	LArmR
		#Part4	:ElectricitySparkBurstSubtle.part
		Part	:ElectricityControl01Subtle.part
		Part	:ElectricityControl02Subtle.part
		Part	:ElectricityInnerGlowSubtle.part
		Part	:ElectricityOuterGlowSubtle.part
		Part	:ElectricityArchSubtle.part
		Part	:ElectricityRingSubtle.part

		Part1	:ChargedBoltA.part
		Part2	:ChargedBoltB.part
		Part3	:ChargedBoltC.part
		Part4	:ChargedBoltD.part
		PMagnet	TargetNodeu	
		LifeSpan	15
	End	

End

######################################################################################


Condition
	On	Cycle
	Time	3
	chance	 .23

	Event

		Ename	TargetNodey
		Type	Local
		At	origin
		
		BhvrOverride	
			StartJitter	1.5 1.5 1.5
		End

		LifeSpan	15
		
	End

	Event
		Type	start
		At	head
		#Part4	:ElectricitySparkBurstSubtle.part
		Part	:ElectricityControl01Subtle.part
		Part	:ElectricityControl02Subtle.part
		Part	:ElectricityInnerGlowSubtle.part
		Part	:ElectricityOuterGlowSubtle.part
		Part	:ElectricityArchSubtle.part
		Part	:ElectricityRingSubtle.part

		Part1	:ChargedBoltA.part
		Part2	:ChargedBoltB.part
		Part3	:ChargedBoltC.part
		Part4	:ChargedBoltD.part
		PMagnet	TargetNodey	
		LifeSpan	15
	End	

End

######################################################################################


Condition
	On	Cycle
	Time	4
	chance	 .23

	Event

		Ename	TargetNodet
		Type	Local
		At	origin
		
		BhvrOverride	
			StartJitter	1.5 1.5 1.5
		End

		LifeSpan	15
		
	End

	Event
		Type	start
		At	Chest
		#Part4	:ElectricitySparkBurstSubtle.part
		Part	:ElectricityControl01Subtle.part
		Part	:ElectricityControl02Subtle.part
		Part	:ElectricityInnerGlowSubtle.part
		Part	:ElectricityOuterGlowSubtle.part
		Part	:ElectricityArchSubtle.part
		Part	:ElectricityRingSubtle.part

		Part1	:ChargedBoltA.part
		Part2	:ChargedBoltB.part
		Part3	:ChargedBoltC.part
		Part4	:ChargedBoltD.part
		PMagnet	TargetNodet	
		LifeSpan	15
	End	

End


End
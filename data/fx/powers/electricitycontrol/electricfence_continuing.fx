#############################################################
## JoltingChain_Hit.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part	:Body_Sparks.part
		Part	:Hand_Ring_Burst.part
		Part	:Hit_Lightning.part
		Part	:Hand_Ring_Burst.part
		Lifespan 30
		Sound	elecfry 80 70 .8
	End
End


Condition
	On	Cycle
	Time	20
	Chance	.75


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End
End

Condition
	On	Cycle
	Time	20
	Chance	.75


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  LarmR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End
End

Condition
	On	Cycle
	Time	10
	Chance	.75


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End
End

Condition
	On	Cycle
	Time	12
	Chance	.75


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  LarmR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End
End

Condition
	On	Cycle
	Time	10
	Chance	.75


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	Hips
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  SpadL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End
End

Condition
	On	Cycle
	Time	12
	Chance	.75


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	Hips
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  SpadR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End
End

Condition
	On	Cycle
	Time	10
	Chance	.75


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	Hips
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  LLegL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End
End

Condition
	On	Cycle
	Time	12
	Chance	.75


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	Hips
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  LLegR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Fence
		Type	posit
		At	root
		Geom	ElectricityFence
		Bhvr	Behaviors/electricityFenceScale1.bhvr

	End

	Event
		Ename	Fence1
		Type	posit
		At	root
		Geom	ElectricityFence
		Bhvr	Behaviors/electricityFenceScale.bhvr

	End

	Event
		Ename	Fence2
		Type	posit
		At	root
		Geom	ElectricityFence
		Bhvr	Behaviors/electricityFenceScale2.bhvr

	End

	Event
		Ename	Fence3
		Type	posit
		At	root
		Geom	ElectricityFence
		Bhvr	Behaviors/electricityFenceScale3.bhvr

	End

	Event
		Ename	Fence4
		Type	posit
		At	root
		Geom	ElectricityFence
		Bhvr	Behaviors/electricityFenceScale4.bhvr

	End

	Event
		Ename	Fence5
		Type	posit
		At	root
		Geom	ElectricityFence
		Bhvr	Behaviors/electricityFenceScale5.bhvr

	End
End

#######################################################################
##Top
###############################################3

Condition
	On 	cycle
	Time 	12
	Chance	.8

	Event
		Ename	Node1a
		Type	start
		At	Fence1
		Altpiv	12
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node2a
		LifeSpan	5
	End

	Event
		Ename	Node2a
		Type	start
		At	Fence1
		Altpiv	10
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node3a
		LifeSpan	5
	End

	Event
		Ename	Node3a
		Type	start
		At	Fence1
		Altpiv	11
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node4a
		LifeSpan	5
	End

	Event
		Ename	Node4a
		Type	start
		At	Fence1
		Altpiv	7
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node5a
		LifeSpan	5
	End

	Event
		Ename	Node5a
		Type	start
		At	Fence1
		Altpiv	8
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node6a
		LifeSpan	5
	End

	Event
		Ename	Node6a
		Type	start
		At	Fence1
		Altpiv	9
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node1a
		LifeSpan	5
	End
End

###################################################################
##Bottem
#######################################

Condition
	On 	cycle
	Time 	8
	Chance	.8

	Event
		Ename	Node1b
		Type	start
		At	Fence2
		Altpiv	1
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node2b
		LifeSpan	5
	End

	Event
		Ename	Node2b
		Type	start
		At	Fence2
		Altpiv	5
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node3b
		LifeSpan	5
	End

	Event
		Ename	Node3b
		Type	start
		At	Fence2
		Altpiv	6
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node4b
		LifeSpan	5
	End

	Event
		Ename	Node4b
		Type	start
		At	Fence2
		Altpiv	2
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node5b
		LifeSpan	5
	End

	Event
		Ename	Node5b
		Type	start
		At	Fence2
		Altpiv	3
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node6b
		LifeSpan	5
	End

	Event
		Ename	Node6b
		Type	start
		At	Fence2
		Altpiv	4
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node1b
		LifeSpan	5
	End
End

#######################################################################
##middle
##################################


Condition
	On 	cycle
	Time 	10
	Chance	.8

		Event
		Ename	Node1c
		Type	start
		At	Fence3
		Altpiv	12
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node2c
		LifeSpan	5
	End

	Event
		Ename	Node2c
		Type	start
		At	Fence3
		Altpiv	10
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node3c
		LifeSpan	5
	End

	Event
		Ename	Node3c
		Type	start
		At	Fence3
		Altpiv	11
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node4c
		LifeSpan	5
	End

	Event
		Ename	Node4c
		Type	start
		At	Fence3
		Altpiv	7
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node5c
		LifeSpan	5
	End

	Event
		Ename	Node5c
		Type	start
		At	Fence3
		Altpiv	8
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node6c
		LifeSpan	5
	End

	Event
		Ename	Node6c
		Type	start
		At	Fence3
		Altpiv	9
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node1c
		LifeSpan	5
	End
End

#######################################################################
##BottemMiddle
###############################################3

Condition
	On 	cycle
	Time 	9
	Chance	.8

	Event
		Ename	Node1h
		Type	start
		At	Fence
		Altpiv	12
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node2h
		LifeSpan	5
	End

	Event
		Ename	Node2h
		Type	start
		At	Fence
		Altpiv	10
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node3h
		LifeSpan	5
	End

	Event
		Ename	Node3h
		Type	start
		At	Fence
		Altpiv	11
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node4h
		LifeSpan	5
	End

	Event
		Ename	Node4h
		Type	start
		At	Fence
		Altpiv	7
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node5h
		LifeSpan	5
	End

	Event
		Ename	Node5h
		Type	start
		At	Fence
		Altpiv	8
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node6h
		LifeSpan	5
	End

	Event
		Ename	Node6h
		Type	start
		At	Fence
		Altpiv	9
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node1h
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	8
	Chance	.8

		Event
		Ename	Node1g
		Type	start
		At	Fence4
		Altpiv	12
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node2g
		LifeSpan	5
	End

	Event
		Ename	Node2g
		Type	start
		At	Fence4
		Altpiv	10
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node3g
		LifeSpan	5
	End

	Event
		Ename	Node3g
		Type	start
		At	Fence4
		Altpiv	11
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node4g
		LifeSpan	5
	End

	Event
		Ename	Node4g
		Type	start
		At	Fence4
		Altpiv	7
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node5g
		LifeSpan	5
	End

	Event
		Ename	Node5g
		Type	start
		At	Fence4
		Altpiv	8
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node6g
		LifeSpan	5
	End

	Event
		Ename	Node6g
		Type	start
		At	Fence4
		Altpiv	9
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node1g
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	9
	Chance	.8

	Event
		Ename	Node1v
		Type	start
		At	Fence5
		Altpiv	1
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node2v
		LifeSpan	5
	End

	Event
		Ename	Node2v
		Type	start
		At	Fence5
		Altpiv	5
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node3v
		LifeSpan	5
	End

	Event
		Ename	Node3v
		Type	start
		At	Fence5
		Altpiv	6
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node4v
		LifeSpan	5
	End

	Event
		Ename	Node4v
		Type	start
		At	Fence5
		Altpiv	2
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node5v
		LifeSpan	5
	End

	Event
		Ename	Node5v
		Type	start
		At	Fence5
		Altpiv	3
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node6v
		LifeSpan	5
	End

	Event
		Ename	Node6v
		Type	start
		At	Fence5
		Altpiv	4
		Part2	:ElectricFenceBolt01.part
		Part3	:ElectricFenceBolt02.part
		Part4	:ElectricFenceBolt06.part
		POther  Node1v
		LifeSpan	5
	End

End

#######################################################################

End

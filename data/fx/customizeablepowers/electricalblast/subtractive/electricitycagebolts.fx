#############################################################
## ElectricityCageBolts.fx
#############################################################

FxInfo
LifeSpan	250

########################################################


Condition
	On 	Time
	Time 	30

	Event
		Type	start
		At	HandR
		Part1	:ChargedBolt06.part
		Part2	:ChargedBolt05.part
		Part3	:ChargedBolt01.part
		Part4	:ChargedBolt02.part
		POther Target
	End

End

Condition
	On 	Time
	Time 	33

	Event
		Type	start
		At	HandR
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	37

	Event
		Type	start
		At	HandR
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	40

	Event
		Type	start
		At	HandR
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	44

	Event
		Type	start
		At	HandR
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	47

	Event
		Type	start
		At	HandR
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	51

	Event

		Type	start
		At	HandR
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part

		POther  Target
	End

End

Condition
	On 	Time
	Time 	54

	Event

		Type	start
		At	HandR
		Part3	:ChargedBolt03.part
		Part4	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	57

	Event

		Type	start
		At	HandR
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part

		POther  Target
	End

End

Condition
	On 	Time
	Time 	61

	Event

		Type	start
		At	HandR
		Part3	:ChargedBolt03.part
		Part4	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	64

	Event

		Type	start
		At	HandR
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part

		POther  Target
	End

End

Condition
	On 	Time
	Time 	67

	Event

		Type	start
		At	HandR
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt06.part
		Part3	:ChargedBolt05.part
		Part4	:ChargedBolt04.part
		POther  Target
	End

End

######################################################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	start
		At	HandL
		Part1	:ChargedBolt06.part
		Part2	:ChargedBolt05.part
		Part3	:ChargedBolt01.part
		Part4	:ChargedBolt02.part
		POther Target
	End

End

Condition
	On 	Time
	Time 	33

	Event
		Type	start
		At	HandL
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	37

	Event
		Type	start
		At	HandL
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	40

	Event
		Type	start
		At	HandL
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	44

	Event
		Type	start
		At	HandL
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	47

	Event
		Type	start
		At	HandL
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	51

	Event

		Type	start
		At	HandL
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part

		POther  Target
	End

End

Condition
	On 	Time
	Time 	54

	Event

		Type	start
		At	HandL
		Part3	:ChargedBolt03.part
		Part4	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	57

	Event

		Type	start
		At	HandL
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part

		POther  Target
	End

End

Condition
	On 	Time
	Time 	61

	Event

		Type	start
		At	HandL
		Part3	:ChargedBolt03.part
		Part4	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	64

	Event

		Type	start
		At	HandL
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part

		POther  Target
	End

End

Condition
	On 	Time
	Time 	67

	Event

		Type	start
		At	HandL
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt06.part
		Part3	:ChargedBolt05.part
		Part4	:ChargedBolt04.part
		POther  Target
	End

End


#############################################################################
##Hands
############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Sound cagebolt2 80 80 .8
		Lifespan 71


	End

	Event
		EName 	Thingy1
		Type	local
		At	HandL
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Lifespan 71

	End


	Event
		EName 	ThingyA
		Type	local
		At	HandR
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Lifespan 71

	End

	Event
		EName 	ThingyB
		Type	local
		At	HandL
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Lifespan 71

	End
End

Condition
	On 	Time
	Time 	25
	Event
		EName 	BurstR
		Type	local
		At	HandR
		Part4	:ElectricitySparkBurst.part
		SOund	lightningbolt2 80 30 .76
		LifeSpan	20

	End

	Event
		EName 	BurstL
		Type	local
		At	HandL
		Part4	:ElectricitySparkBurst.part
		LifeSpan	20

	End

End

Condition
	On 	PrimeHit

	Event

		EName	Prime
		Type	destroy


	End

End

#############################################################################

End
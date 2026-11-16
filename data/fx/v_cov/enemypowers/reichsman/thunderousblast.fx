#########################################################
##	ThunderousBlast.fx
########################################################

FxInfo
LifeSpan	200

########################################################

Condition
	On 	Time
	Time	0

	Event
		Type	Local
		At 	Origin
		Sound ReichsmanSapWill_01 100 100 1.0

	End
End


Condition
	On 	Time
	Time 	36

	Event
		EName 	1	#Prime
		Type	start
		At	HandL
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr

		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	39

	Event
		EName 	Prime
		Type	start
		At	HandL
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr

		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	45

	Event
		EName 	2
		Type	start
		At	HandL
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr

		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	52

	Event
		EName 	2.3
		Type	start
		At	HandL
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr

		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	59

	Event
		EName 	Prime2
		Type	start
		At	HandL
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr

		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		EName 	1
		Type	Destroy

	End


	Event

		Type	Local
		At	Target
#		Part2	:ShockFieldGlow.part

		lifespan	5
	End

End

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy

	End

	Event
		EName 	2
		Type	Destroy

	End

	Event
		EName 	2.3
		Type	Destroy

	End

End

Condition
	On 	Prime3Hit

	Event
		EName 	Prime3
		Type	Destroy

	End

	Event
		EName 	3
		Type	Destroy

	End

End

Condition
	On 	Prime4Hit

	Event
		EName 	Prime4
		Type	Destroy

	End

	Event
		EName 	5
		Type	Destroy

	End

	Event
		EName 	4
		Type	Destroy

	End

End

############################################################################################


########################################################

Condition
	On 	Time
	Time 	34

	Event
		EName 	3
		Type	start
		At	HandR
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr

		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	40

	Event
		EName 	Prime3
		Type	start
		At	HandR
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr

		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	43

	Event
		EName 	4
		Type	start
		At	HandR
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr

		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	53.5

	Event
		EName 	5
		Type	start
		At	HandR
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr

		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	62

	Event
		EName 	Prime4
		Type	start
		At	HandR
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr

		Magnet	Target
		Lookat	Target

	End

End

############################################################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	start
		At	HandR
		Part1	:QuickBolt06.part
		Part2	:QuickBolt05.part
		Part3	:QuickBolt01.part
		Part4	:QuickBolt02.part
		Part5	:BeamGlow.part
		POther Target
	End

End

Condition
	On 	Time
	Time 	33

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	37

	Event
		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	40

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	44

	Event
		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	47

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End


Condition
	On 	Time
	Time 	51

	Event
		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	55

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	59

	Event
		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	61

	Event

		Type	start
		At	HandR
		Part3	:QuickBolt06.part
		Part4	:QuickBolt02.part
		Part5	:QuickBolt04.part
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
		Part1	:QuickBolt06.part
		Part2	:QuickBolt05.part
		Part3	:QuickBolt01.part
		Part4	:QuickBolt02.part
		Part5	:BeamGlow.part

		POther Target
	End

End

Condition
	On 	Time
	Time 	33

	Event
		Type	start
		At	HandL
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	38

	Event
		Type	start
		At	HandL
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	41

	Event
		Type	start
		At	HandL
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End


Condition
	On 	Time
	Time 	55

	Event
		Type	start
		At	HandL
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		Part2	:QuickBolt03.part
		Part3	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	48

	Event
		Type	start
		At	HandL
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	51

	Event
		Type	start
		At	HandL
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End



Condition
	On 	Time
	Time 	58

	Event
		Type	start
		At	HandL
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	61

	Event

		Type	start
		At	HandL
		Part3	:QuickBolt06.part
		Part4	:QuickBolt02.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

#############################################################################
##Hands
############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Lifespan 103

	End

	Event
		EName 	ThingyA
		Type	local
		At	HandR
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:HandBeams.part
		Part5	:HandBeamsSub.part
		Lifespan 103


	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName 	Thingy1
		Type	local
		At	HandL
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Lifespan 90

	End

	Event
		EName 	ThingyB
		Type	local
		At	HandL
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:HandBeams.part
		Part5	:HandBeamsSub.part
		Lifespan 90

	End
End

#########################################################################
##Spark Bursts
##############################################################

Condition
	On 	Time
	Time 	26
	Event
		EName 	BurstR
		Type	local
		At	HandR
		Part4	:ElectricitySparkBurst.part
#		SOund	lightningbolt2 80 30 .55
		LifeSpan	20

	End

End

Condition
	On 	Time
	Time 	45

	Event
		EName 	BurstR
		Type	local
		At	HandL
		Part4	:ElectricitySparkBurst.part
#		SOund	lightningbolt2 80 30 .55
		LifeSpan	20

	End

End

Condition
	On 	Time
	Time 	72

	Event
		EName 	BurstR
		Type	local
		At	HandR
		Part4	:ElectricitySparkBurst.part
# 		SOund	lightningbolt2 80 30 .6
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

#########################################################################



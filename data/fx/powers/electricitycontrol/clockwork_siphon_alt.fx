#########################################################
##	clockwork siphon
#########################################################
FxInfo


#LifeSpan	150

##################################################################################################################
#############################################     Right Arm     ##################################################
##################################################################################################################


Condition
	On 	Time
	Time 	0

	Event
		Ename   TargetGoal
		Type	Local
		At	Head
		
		Bhvr	:Clockwork_SiphonTargetRight.bhvr
	End
	Event
		Ename   TargetRight
		Type	Start
		At	TargetGoal
		
#		Bhvr	:Clockwork_SiphonTargetRight.bhvr
#		Geom	TESTING_Target
#		lifespan 100
	End
	
End



Condition
	On 	Cycle
	Time 	14

	Event
		Type	start
		At	HandR
		Part1	:Clockwork_ChargedBolt06.part
		Part2	:Clockwork_ChargedBolt05.part
		Part3	:Clockwork_ChargedBolt01.part
		Part4	:Clockwork_ChargedBolt02.part
		POther  TargetRight
		lifespan 7
		
	End		
	
End

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	HandR
		Part4	:Clockwork_ChargedBolt03.part
		Part5	:Clockwork_ChargedBolt04.part
		POther  TargetRight
		lifespan 30
	End

End

Condition
	On 	Time
	Time 	42

	Event
		Type	start
		At	HandR
		Part4	:Clockwork_ChargedBolt03.part
		Part5	:Clockwork_ChargedBolt04.part
		POther  TargetRight
		Lifespan 30
	End

End


Condition
	On 	Cycle
	Time 	80

	Event
		Type	start
		At	HandR
		Part2	:Clockwork_ChargedBolt01.part
		Part3	:Clockwork_ChargedBolt02.part
		POther  TargetRight	
		Lifespan 25
	End

End

##################################################################################################################
#############################################     Left  Arm     ##################################################
##################################################################################################################


Condition
	On 	Time
	Time 	0

	Event
		Ename   TargetGoal_Left
		Type	Local
		At	Head
		Bhvr	:Clockwork_SiphonTargetLeft.bhvr
	End

	Event
		Ename   TargetLeft
		Type	Start
		At	TargetGoal_Left
		
#		Geom	WepR_torch_sm
#		lifespan 100
	End
	
End



Condition
	On 	Cycle
	Time 	14

	Event
		Type	start
		At	HandL
		Part1	:Clockwork_ChargedBolt06.part
		Part2	:Clockwork_ChargedBolt05.part
		POther  TargetLeft
		lifespan 7
		
	End		
	
End

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	HandL
		Part4	:Clockwork_ChargedBolt03.part
		Part5	:Clockwork_ChargedBolt04.part
		POther  TargetLeft
		lifespan 30
	End

End

Condition
	On 	Time
	Time 	42

	Event
		Type	start
		At	HandL
		Part4	:Clockwork_ChargedBolt03.part
		Part5	:Clockwork_ChargedBolt04.part
		POther  TargetLeft
		Lifespan 30
	End

End


Condition
	On 	Cycle
	Time 	80

	Event
		Type	start
		At	HandL
		Part2	:Clockwork_ChargedBolt01.part
		Part3	:Clockwork_ChargedBolt02.part
		POther  TargetLeft
		Lifespan 25
	End

End


##################################################################################################################
#############################################     Hand Glow     ##################################################
##################################################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Thingy
		Type	local
		At	HandR

		Bhvr	:Clockwork_Siphon_alt_R.bhvr
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
#		Part3	:Clockwork_ElectricityInnerGlow.part
		SOund	clocksiphon_loop 50 30 .55
		
	End

	Event
		EName 	Thingy1
		Type	local
		At	HandL

		Bhvr	:Clockwork_Siphon_alt.bhvr
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
#		Part3	:Clockwork_ElectricityInnerGlow.part
#		SOund	lightningbolt3 50 30 .45
#		Lifespan 50
	End


	Event
		EName 	ThingyA
		Type	local
		At	HandR

		Bhvr	:Clockwork_Siphon_alt_R.bhvr
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
			
	End

	Event
		EName 	ThingyB
		Type	local
		At	HandL

		Bhvr	:Clockwork_Siphon_alt.bhvr
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
	End
End

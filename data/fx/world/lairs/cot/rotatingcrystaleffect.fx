#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	root
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	P
		Type	Local 
		At	root
		Geom	cot_LargeCrystalSpin
		Bhvr	behaviors/Rune_SpinFast.bhvr
		Sound Crystal3_loop 70 70 .9
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	P2
		Type	Local 
		At	root
		Part1	:CrystalMist2.part
		Part2	:CrystalMist1.part
		Part3	:CrystalSparks.part
		Part5	:CrystalSparks.part
		Part4	:CrystalSparks.part
		Sound RuneShimmer1_loop 110 110 .6
	End

End

Condition
	On 	Time
	Time 	0
	#Chance	.4

	Event
		EName 	Pgg
		Type	Local 
		At	root
		Part1	:Ghosts.part
		
	End

End

####################################################
##Sparkles
#####################################################

Condition
	On 	Time
	Time 	3
	Chance	.5
	Event
		EName 	Shine1
		Type	Local 
		At	P
		AltPiv	1
		Part1	:ShineCoreFast.part
		Part2	:ShineRingsFast.part
	End

End
	
Condition
	On 	Time
	Time 	14
	Chance	.7

	Event
		EName 	Shine2
		Type	Local 
		At	P
		AltPiv	2
		Part1	:ShineCoreFast.part
		Part2	:ShineRingsFast.part
	End
End
	
Condition
	On 	Time
	Time 	2
	Chance	.5
	Event
		EName 	Shine3
		Type	Local 
		At	P
		AltPiv	3
		Part1	:ShineCoreFast.part
		Part2	:ShineRingsFast.part
	End
End
	
Condition
	On 	Time
	Time 	1
	Chance	.55
	Event
		EName 	Shine4
		Type	Local 
		At	P
		AltPiv	4
		Part1	:ShineCoreFast.part
		Part2	:ShineRingsFast.part
	End
End
	
Condition
	On 	Time
	Time 	0
	Chance	.5
	Event
		EName 	Shine5
		Type	Local 
		At	P
		AltPiv	5
		Part1	:ShineCoreFast.part
		Part2	:ShineRingsFast.part
	End

End
	
Condition
	On 	Time
	Time 	7
	Chance	.5
	Event
		EName 	Shine6
		Type	Local 
		At	P
		AltPiv	6
		Part1	:ShineCoreFast.part
		Part2	:ShineRingsFast.part
	End
End
	
Condition
	On 	Time
	Time 	9
	Chance	.55
	Event
		EName 	Shine7
		Type	Local 
		At	P
		AltPiv	7
		Part1	:ShineCoreFast.part
		Part2	:ShineRingsFast.part
	End

End
	
Condition
	On 	Time
	Time 	11
	Chance	.7

	Event
		EName 	Shine8
		Type	Local 
		At	P
		AltPiv	8
		Part1	:ShineCoreFast.part
		Part2	:ShineRingsFast.part
	End

#####################################################
##Base&Core
###################################################
	
	Event
		EName 	CoreSparkley
		Type	Local 
		At	P
		AltPiv	10
		Part1	:Sparkley.part
		Part2	:SparkleyCore.part
	End
	
	Event
		EName 	CoreSparkley
		Type	Local 
		At	P
		AltPiv	9
		Part3	:CrystalSparksBig.part
	End
	
End

End			
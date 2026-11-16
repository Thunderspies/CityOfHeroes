#########################################################
##	EBlast
########################################################
FxInfo

Input  
	Inpname	Chest
End

LifeSpan	120

#################################################################
##Hands
#################################################################



Condition
	On	Time
	Time	0

	Event
		EName 	HitFX
		Type	local 
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01Sml.part
		Part2	Powers/MartialArts/MAHitStar02Sml.part
		Part3	Powers/MartialArts/MAHitStar03Sml.part
		Part4	Powers/MartialArts/MAHitGlow01Sml.part
		Part5	Powers/MartialArts/MAHitRays01Sml.part
		#Part	Powers/MartialArts/MartialArtsHitRaysSmall.part
		LifeSpan	10
		
	End

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	:ElectricityGlow3.part
		Part2	:ElectricityGlowStar3.part
		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchSmalla.part
		Sound pp12 50 50 .7
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
	End
End

	Condition
	On 	Time
	Time 	23
	Event
		EName 	HandGlow1
		Type	Destroy
				
	End

End



End

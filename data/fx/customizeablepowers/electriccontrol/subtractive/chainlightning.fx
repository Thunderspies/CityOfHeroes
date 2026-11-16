#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	HandR
End

Input  
	Inpname	HandL
End

#LifeSpan	200

########################################################

Condition
	On 	Time
	Time 	15

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Sound ChargeLight3 80 80 .7
		LifeSpan	50

	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		LifeSpan	50

	End


	Event
		EName 	Thingy3
		Type	local
		At	HandR
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:ElectricityArchInvert.part
		LifeSpan	50

	End

	Event
		EName 	Thingy4
		Type	local
		At	HandL
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:ElectricityArchInvert.part
		LifeSpan	50
		
	End

End

Condition
	On 	Time
	Time 	24

	Event
		EName 	BurstL
		Type	local
		At	HandL
		Part4	:ElectricitySparkBurst.part
		LifeSpan	20

	End

	Event
		EName 	BurstL
		Type	local
		At	HandR
		Part4	:ElectricitySparkBurst.part
		LifeSpan	20

	End
End

#################################################################################

Condition
	On 	Time
	Time 	40

	Event
		EName	lightning
		Type	local
		At	root
		Part2	:ChainLightningArchs3.part
		Part3	:ChainLightningArchs2.part
		Part4	:ChainLightningArchs1.part
		Part5	:ChainLightningArchs.part
		#Part1	:ChainLightningArchz.part
		Sound ChainLight2_loop 100 100 .8
	End

	Event
		EName	lightning4
		Type	local
		At	root
				
		Part1	:ChainLightningArchsSpinner.part
		#Part2	:ChainLightningRing.part
		#Part3	:ChainLightningThinRing.part
		

		#LifeSpan	30
	End
End


Condition
	On 	cycle
	Time 	33
	Chance	.8

	Event
		EName	lightning2
		Type	local
		At	root
				
		#Part1	:ChainLightningArchsSpinner.part
		Part2	:ChainLightningRing2.part
		#Part3	:ChainLightningThinRing.part

		LifeSpan	50
	End

End



End
#########################################################
## Electric Aura - Lighting Field
########################################################

FxInfo

########################################################

Condition
	On 	Time
	Time 	15

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	:Red_ElectricityControl01.part
		Part2	:Red_ElectricityControl02.part
		Part3	:Red_ElectricityInnerGlow.part
		Part1	:Red_ElectricityControl01white.part
		Part2	:Red_ElectricityControl02white.part
		Sound lightningfield2 80 80 .6
		LifeSpan	30
	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL
		Part1	:Red_ElectricityControl01.part
		Part2	:Red_ElectricityControl02.part
		Part3	:Red_ElectricityInnerGlow.part
		Part1	:Red_ElectricityControl01white.part
		Part2	:Red_ElectricityControl02white.part
		LifeSpan	30
	End


	Event
		EName 	Thingy3
		Type	local
		At	HandR
		Part1	:Red_ElectricityOuterGlow.part
		Part2	:Red_ElectricityArch.part
		Part3	:Red_ElectricityRing.part
		Part4	:Red_ElectricityArchInvert.part
		LifeSpan	30
	End

	Event
		EName 	Thingy4
		Type	local
		At	HandL
		Part1	:Red_ElectricityOuterGlow.part
		Part2	:Red_ElectricityArch.part
		Part3	:Red_ElectricityRing.part
		Part4	:Red_ElectricityArchInvert.part
		LifeSpan	30
	End
End

Condition
	On 	Time
	Time 	24

	Event
		EName 	BurstL
		Type	local
		At	HandL
		Part4	:Red_ElectricitySparkBurst.part
		LifeSpan	20
	End

	Event
		EName 	BurstL
		Type	local
		At	HandR
		Part4	:Red_ElectricitySparkBurst.part
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
		Part2	:Red_ChainLightningArchs3.part
		Part3	:Red_ChainLightningArchs2.part
		Part4	:Red_ChainLightningArchs1.part
		Part5	:Red_ChainLightningArchs.part
		Part1	:Red_ChainLightningArchsWhite.part
		Part1	:Red_ChainLightningArchsWhite2.part
		#Lifespan 350
	End
End

Condition
	On 	Time
	Time 	40

	Event
		EName	lightning4
		Type	local
		At	root
				
		Part2	:Red_ChainLightningRing.part
		Part3	:Red_ChainLightningThinRing.part
	End
End

Condition
	On 	cycle
	Time 	40

	Event
		EName	lightning4
		Type	local
		At	root
				
		Part1	:Red_ChainLightningArchsSpinner.part
		Part2	:Red_ChainLightningRing.part
		Part3	:Red_ChainLightningThinRing.part
		LifeSpan	40
	End
End


#Condition
#	On 	Time
#	Time 	40
#
#	Event
#		Type	local
#		At	root
#		
#		Bhvr	:Red_LightningFieldSplat.bhvr		
#		Splat	electricring1copy.tga
#
#		LifeSpan	20
#	End
#
#End

#Condition
#	On 	cycle
#	Time 	30
#	Chance	.8
#
#	Event
#		EName	lightning2
#		Type	local
#		At	root
#		
#		Bhvr	:Red_LightningFieldSplat.bhvr		
#		Splat	electricring1copy.tga
#		LifeSpan	10
#	End
#End

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	Origin
		Sound elecArmor3_loop 70 70 .34
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 350
	End
End

End
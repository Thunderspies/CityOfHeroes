#########################################################
##	forceBubble
########################################################
FxInfo


#################################################################################

Condition
	
	Event
		EName	lightning
		Type	local
		At	root
		Part2	:IONChainLightningArchs3.part
		Part3	:IONChainLightningArchs2.part
		Part	:IONLightningArchs.part
		Part	:IONLightningArchsPink.part
		Part4	:IONChainLightningArchsPink.part
			#Part5	:ChainLightningArchs.part
			#Part1	:ChainLightningArchz.part
		Sound ChainLight2_loop 80 80 .5
	End

	Event
		EName	lightning4
		Type	local
		At	root
				
		#Part1	:ChainLightningArchsSpinner.part
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
		#Part2	:ChainLightningRing2.part
			#Part3	:ChainLightningThinRing.part

		LifeSpan	50
	End

End



End
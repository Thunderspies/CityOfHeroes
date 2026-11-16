#############################################################
## ForceFieldGenerator.fx
#############################################################

FxInfo

##########################################################

Condition

	Event
		Ename	Body
		Type	local
		At	chest

		Bhvr	:ForceScaleScale.BHVR
		geom	HunterSeeker

	End


	Event
		Ename	lightTop
		Type	local
		At	Body
		altpiv	1


		Part	:RedLightCenter2.part
		Part1	Generic/LightCenter.part
		Part2	Generic/LightGlow.part
	End

End

Condition
	On	Time
	Time	4

	Event
		Ename	lightBottem
		Type	local
		At	Body
		altpiv	2

		Part	:BlueLightCenter2.part

		Part1	Generic/LightCenter.part
		Part2	Generic/LightGlow.part

	End

End

#############################################################

End
#########################################################
##	chill touch hit
FxInfo

#LifeSpan	100

##################################


Condition
	
	Event
		EName	1
		Type	Local
		At	Hips
		part1	:endurance01.part
#		bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	10
	End

	Event
		EName	1
		Type	Local
		At	Hips
		Part	:ElectricityArch.part
		Part	:ElectricityPOPS.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	2
		Type	Local
		At	FootR
		part1	:enduranceBodyGlows.part
		Part	:ElectricityArch.part
		Part	:ElectricityPOPS.part
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:enduranceMotionGlows.part
		Part	:ElectricityArch.part
		Part	:ElectricityPOPS.part

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:enduranceMotionGlows.part
		Part	:ElectricityArch.part
		Part	:ElectricityPOPS.part
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:enduranceBodyGlows.part
		Part	:ElectricityArch.part
		Part	:ElectricityPOPS.part
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:enduranceMotionGlows.part
		Part	:ElectricityArch.part
		Part	:ElectricityPOPS.part

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:enduranceMotionGlows.part
		Part	:ElectricityArch.part
		Part	:ElectricityPOPS.part
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Hair
		part2	:enduranceHeadGlows.part
		Part	:ElectricityArch.part
		Part	:ElectricityPOPS.part
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:enduranceArmMotionGlows.part
		part2	:enduranceArmMotionGlows2.part
		Part	:ElectricityArch.part
		Part	:ElectricityPOPS.part

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:enduranceArmMotionGlows.part
		part2	:enduranceArmMotionGlows2.part
		Part	:ElectricityArch.part
		Part	:ElectricityPOPS.part
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:enduranceArmGlows.part
		Part	:ElectricityArch.part
		Part	:ElectricityPOPS.part
	
	End






	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:enduranceArmMotionGlows.part
		part2	:enduranceArmMotionGlows2.part
		Part	:ElectricityArch.part
		Part	:ElectricityPOPS.part

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:enduranceArmMotionGlows.part
		part2	:enduranceArmMotionGlows2.part
		Part	:ElectricityArch.part
		Part	:ElectricityPOPS.part
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:enduranceArmGlows.part
		Part	:ElectricityArch.part
		Part	:ElectricityPOPS.part
	End



End

#Condition
#	On	Time
#	Time	60
#	
#	Event
#		EName	all
#		Type	Destroy
#				
#	End
#
#End


End				
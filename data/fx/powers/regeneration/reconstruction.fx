#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	InpName	FootL
End

Input  
	InpName	FootR
End

Input  
	InpName	ULEGL
End

Input  
	InpName	ULEGR
End

Input  
	InpName	LLEGL
End

Input  
	InpName	LLEGR
End

Input  
	InpName	wepR
End

Input  
	InpName	CHEST
End

Input  
	InpName	Hair
End

Lifespan	65

########################################################

Condition
	On 	Time
	Time 	10
	
	Event
		EName	dummy2
		Type	Local 
		At	chest
		Part2	:SparkliesTrail.part
				
	End

	Event
		EName	dummy
		Type	Local 
		At	chest
		
		Part1	:core.part
		Part2	:Electrons.part
		Part3	:StarStreaks.part
		Part4	:ReconStreaksDown.part
		Part5	:ReconStreaksRight.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		Sound reconstruction 60 60 .8
		Lifespan	35
	End

	Event
		EName	dummy2
		Type	Local 
		At	chest
		
		Part1	:Integration01.part
		Part2	:CoreRings.part
		Part3	:ReconStreaksRighta.part
		Part4	:ReconStreaksLefta.part
		Part5	:ReconStreaksLeft.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	35
	End

End

Condition
	On	Time
	Time	15
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	:RegenBodyGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:RegenMotionGlows.part
		PMagnet Knee1
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:RegenMotionGlows.part
		PMagnet FootL
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:RegenBodyGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:RegenMotionGlows.part
		PMagnet Kneer
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:RegenMotionGlows.part
		PMagnet FootR
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

End


End
##################################


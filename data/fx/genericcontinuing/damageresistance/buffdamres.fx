#########################################################
##	template

FxInfo

Input
	InpName Origin
End

Input  
	InpName	Target
End




#########################################################
####################### AUDIO ###########################
#########################################################




#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	
	On	Time
	Time	0

	Event	
		ENAME	Chestnode
		Type	Local
		At	Origin
		Geom	ChestPivotsBuffToHit
	End

	Event	
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	2
		part	:BuffDamResShield.part
		part	:BuffDamResChevron01.part
		part	:BuffDamResChevron03.part
		part	:BuffDamResChevron02.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	Back
		Type	Local
		At	Chestnode
		AltPiv	3
		part	:BuffDamResShield.part
		part	:BuffDamResChevron01.part
		part	:BuffDamResChevron03.part
		part	:BuffDamResChevron02.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	5
		part	:BuffDamResShield.part
		part	:BuffDamResChevron01.part
		part	:BuffDamResChevron03.part
		part	:BuffDamResChevron02.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	Back
		Type	Local
		At	Chestnode
		AltPiv	6
		part	:BuffDamResShield.part
		part	:BuffDamResChevron01.part
		part	:BuffDamResChevron03.part
		part	:BuffDamResChevron02.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
	
	End

End


End
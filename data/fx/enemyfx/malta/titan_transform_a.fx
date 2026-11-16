#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	LarmR
End

Input  
	Inpname	LarmL
End

Input  
	Inpname	Chest
End


Flags InheritAlpha

##################################################################

LifeSpan 36

Condition
	On	Time
	Time	0
	Event
		EName 	Jumping_mek
		Type	Local
		At	Root
		Anim	FX_Titan_transform_spawn
		Bhvr	Behaviors/quick_fadein.bhvr
		Sound transform1 80 80 .75

	End

End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0
	
	Event	
		ENAME	NeckNode
		Type	Local
		At	Jumping_mek
		AnimPiv	Neck

	
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Jumping_mek
		AnimPiv	Hips
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops01.part
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops02.part
		part	ENEMYFX\Malta\TitanTransformFX\TransformArcs01.part
		Pmagnet NeckNode
		POther  NeckNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	ChestNode
		Type	Local
		At	Jumping_mek
		AnimPiv	Chest
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Jumping_mek
		AnimPiv	Hair
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops01.part
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops02.part
		part	ENEMYFX\Malta\TitanTransformFX\TransformArcs01.part
		Pmagnet ChestNode
		POther  ChestNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LarmRNode
		Type	Local
		At	Jumping_mek
		AnimPiv	LarmR
	
	End

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	Jumping_mek
		AnimPiv	SpadR
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops01.part
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops02.part
		part	ENEMYFX\Malta\TitanTransformFX\TransformArcs01.part
		Pmagnet LarmRNode
		POther  LarmRNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	Jumping_mek
		AnimPiv	WepR
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops01.part
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops02.part
		part	ENEMYFX\Malta\TitanTransformFX\TransformArcs01.part
		Pmagnet LarmRnode
		POther  LarmRNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LarmLNode
		Type	Local
		At	Jumping_mek
		AnimPiv	LarmL
	
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	Jumping_mek
		AnimPiv	SpadL
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops01.part
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops02.part
		part	ENEMYFX\Malta\TitanTransformFX\TransformArcs01.part
		Pmagnet LarmLNode
		POther  LarmLNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	Jumping_mek
		AnimPiv	WepL
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops01.part
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops02.part
		part	ENEMYFX\Malta\TitanTransformFX\TransformArcs01.part
		Pmagnet LarmLNode
		POther  LarmLNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LleglNode
		Type	Local
		At	Jumping_mek
		AnimPiv	Llegl
	
	End

	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	Jumping_mek
		AnimPiv	UlegL
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops01.part
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops02.part
		part	ENEMYFX\Malta\TitanTransformFX\TransformArcs01.part
		Pmagnet LleglNode
		POther  LleglNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0	
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	Jumping_mek
		AnimPiv	FootL
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops01.part
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops02.part
		part	ENEMYFX\Malta\TitanTransformFX\TransformArcs01.part
		Pmagnet LleglNode
		POther  LleglNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LlegRNode
		Type	Local
		At	Jumping_mek
		AnimPiv	LlegR
	
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	Jumping_mek
		AnimPiv	UlegR
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops01.part
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops02.part
		part	ENEMYFX\Malta\TitanTransformFX\TransformArcs01.part
		Pmagnet LlegRNode
		POther  LlegRNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	Jumping_mek
		AnimPiv	FootR
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops01.part
		part	ENEMYFX\Malta\TitanTransformFX\ElectricPops02.part
		part	ENEMYFX\Malta\TitanTransformFX\TransformArcs01.part
		Pmagnet LlegRNode
		POther  LlegRNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


End




End
##################################


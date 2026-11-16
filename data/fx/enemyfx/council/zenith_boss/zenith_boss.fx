#########################################################
##	template

FxInfo


Condition
	On	Time
	Time	0
	
	Event	
		ENAME	LarmRNode
		Type	Local
		At	LarmR
	
	End

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsSmall01.part
		Pmagnet LarmRNode
		POther  LarmRNode
		Lifespan 0
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsSmall01.part
		Pmagnet LarmRnode
		POther  LarmRNode
		Lifespan 0
		
	End

	Event	
		ENAME	LarmLNode
		Type	Local
		At	LarmL
	
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsSmall01.part
		Pmagnet LarmLNode
		POther  LarmLNode
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:ElectricArcsLarge01.part
		part	:ElectricArcsSmall01.part
		Pmagnet LarmLNode
		POther  LarmLNode
		Lifespan 0
		
	End

End

End

	
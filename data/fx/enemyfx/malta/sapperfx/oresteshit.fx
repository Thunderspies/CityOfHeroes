#########################################################
##	template

FxInfo


Lifespan 72

#########################################################
####################### AUDIO ###########################
#########################################################

Condition

	On 	Time	
	Time	0

	Event
		Type Local
		At origin
		Sound teletrans4 70 70 .73
	End

End




#########################################################
#################### Soul_Tickler_ GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0
	
	Event	
		ENAME	NeckNode
		Type	Local
		At	Neck

	
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:Soul_Tickler_ArcsLarge01.part
		part	:Soul_Tickler_ArcsSmall01.part
		Pmagnet NeckNode
		POther  NeckNode
		#Bhvr	Behaviors/TelportationFade.bhvr

	End

	Event	
		ENAME	ChestNode
		Type	Local
		At	Chest
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:Soul_Tickler_ArcsLarge01.part
		part	:Soul_Tickler_ArcsSmall01.part
		Pmagnet ChestNode
		POther  ChestNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End
	
	Event	
		ENAME	LarmRNode
		Type	Local
		At	LarmR
	
	End

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:Soul_Tickler_ArcsLarge01.part
		part	:Soul_Tickler_ArcsSmall01.part
		Pmagnet LarmRNode
		POther  LarmRNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:Soul_Tickler_ArcsLarge01.part
		part	:Soul_Tickler_ArcsSmall01.part
		Pmagnet LarmRnode
		POther  LarmRNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		
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
		part	:Soul_Tickler_ArcsLarge01.part
		part	:Soul_Tickler_ArcsSmall01.part
		Pmagnet LarmLNode
		POther  LarmLNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:Soul_Tickler_ArcsLarge01.part
		part	:Soul_Tickler_ArcsSmall01.part
		Pmagnet LarmLNode
		POther  LarmLNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End

	Event	
		ENAME	LleglNode
		Type	Local
		At	Llegl
	
	End

	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:Soul_Tickler_ArcsLarge01.part
		part	:Soul_Tickler_ArcsSmall01.part
		Pmagnet LleglNode
		POther  LleglNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:Soul_Tickler_ArcsLarge01.part
		part	:Soul_Tickler_ArcsSmall01.part
		Pmagnet LleglNode
		POther  LleglNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End

	Event	
		ENAME	LlegRNode
		Type	Local
		At	LlegR
	
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:Soul_Tickler_ArcsLarge01.part
		part	:Soul_Tickler_ArcsSmall01.part
		Pmagnet LlegRNode
		POther  LlegRNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:Soul_Tickler_ArcsLarge01.part
		part	:Soul_Tickler_ArcsSmall01.part
		Pmagnet LlegRNode
		POther  LlegRNode
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End


End


End		
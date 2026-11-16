#########################################################
##	template

FxInfo

Input
	InpName Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Root
End

Input  
	InpName	Head
End

Input  
	InpName	WepL
End

Input  
	InpName	WepR
End



Lifespan 100

#########################################################
####################### AUDIO ###########################
#########################################################




#########################################################
###################  Hand Casts  ########################
#########################################################


Condition
	On	Time
	Time	12

	Event	
		ENAME	LeftHand
		Type	Posit
		At	WepL
		part	:IllusCastGlow01.part
		Sound deceive1 100 100 .7
		Lifespan 48
	
	End

	Event	
		ENAME	RightHand
		Type	Local
		At	WepR
		part	:IllusCastGlow01.part
		Lifespan 48
	
	End

End

Condition
	On	Time
	Time	25

	Event	
		ENAME	LeftHand
		Type	Posit
		At	WepL
		part	:IllusCastGlow02.part
		Lifespan 20
	
	End

	Event	
		ENAME	RightHand
		Type	Local
		At	WepR
		part	:IllusCastGlow02.part
		Lifespan 20
	
	End

End

Condition
	On	Time
	Time	60

	Event	
		ENAME	LeftHand
		Type	Posit
		At	WepL
		part	:IllusBlindGlow02.part
		Lifespan 10
	
	End

	Event	
		ENAME	RightHand
		Type	Local
		At	WepR
		part	:IllusBlindGlow02.part
		Lifespan 11
	
	End

End

#########################################################
#################### OverHead  1 ########################
#########################################################



Condition
	On	Time
	Time	0
	
	Event	
		ENAME	Overhead
		Type	Local
		At	Root
		Geom	Overheaddummy
	End

End

Condition
	On	Time
	Time	55
	
	Event
		Type	Local
		At 	Origin
		Sound IllusionFlash2 100 100 .95
	End
End

Condition
	On	Time
	Time	60
	
	Event
		Type	Local
		At 	Origin
		Sound explohit 100 100 .6
	End
End


Condition
	On	Time
	Time	65

	Event	
		ENAME	Flash01
		Type	Local
		At	Overhead
		AltPiv	1
		part	:IllusFlashFlash01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 7
	
	End

End

Condition
	On	Time
	Time	62

	Event	
		ENAME	FlashLines
		Type	Local
		At	Overhead
		AltPiv	1
		part	:IllusFlashFlash02.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 12
	
	End

End

Condition
	On	Time
	Time	62

	Event	
		ENAME	FlashGlows
		Type	Local
		At	Overhead
		AltPiv	1
		part	:IllusFlashFlash03.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 13
	
	End

End


End		
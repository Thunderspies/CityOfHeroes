#########################################################
##	
##

FxInfo


Input  
	InpName	Mystic
End

Input  
	InpName	WepL
End

Input  
	InpName	Root
End

Input  
	InpName	head
End

##Lifespan	80

##################################
Condition
	On 	Time
	Time 	1


	Event
		EName	Flashlight
		Type	Local
		At	WepL
		Geom	GEO_WepL_Flashlight_01
	End


	Event
		EName	Lightbeam
		Type	Local
		At	WepL
		Geom	Flashlight_beam__CarHeadLights

	End


	Event
		EName	lightball
		Type	Local
		At	Flashlight
		Geom	Flashlight_light__CarHeadFlare
		Altpiv   1


	End




	
End

End				
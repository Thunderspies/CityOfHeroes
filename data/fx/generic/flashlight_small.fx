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
		BhvrOverride
			Scale 0.7 0.7 0.7
			PYRRotate 0 180 0
		End
		Geom	GEO_WepL_Flashlight_01
	End

#	Event
#		EName	Spotlight
#		Type	Local
#		At	Mystic
#		BhvrOverride
#			PYRRotate 0 180 0
#		End
#		Geom	Flashlight_spotlight
#
#	End



	Event
		EName	Lightbeam
		Type	Local
		At	WepL
		BhvrOverride
			PYRRotate 0 180 0
		End
		Geom	Flashlight_beam__CarHeadLights

	End


	Event
		EName	lightball
		Type	Local
		At	Flashlight
		BhvrOverride
			PYRRotate 0 180 0
		End
		Geom	Flashlight_light__CarHeadFlare
		Altpiv   1


	End





End

End
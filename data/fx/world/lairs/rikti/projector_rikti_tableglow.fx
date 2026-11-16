#########################################################
##	Rikti Table Glow Projector.    -Nduguid         ##
#########################################################
FxInfo

Condition
	Event
		Type	start
		At	origin
		Splat	Rikti_Computer_Table_Splat.tga
		BhvrOverRide
			FadeInLength		0
			FadeOutLength		0
			Scale			12 12 12
			SplatFlags		ADDITIVE
			SplatSetBack		0    #0 -> 1.0 splatSetBack = what percent of the splat should be behind the node, and what in front
			SplatFalloffType	Down			# UP DOWN NONE BOTH	
			SplatFadeCenter		1			# only for UP and DOWN falloff
			Alpha			100
		
			StartColor		255 255 255
		End
	End
End


Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Origin
		Sound RiktiPwrBoat7_loop 20 20 .04
		
	End
End


End	

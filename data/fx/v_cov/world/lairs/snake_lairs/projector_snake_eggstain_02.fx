#########################################################
##	Arachnoid Cob Web Projector.    -Rroth         ##
#########################################################
FxInfo

Condition
	Event
		Type	start
		At	origin
		Splat	Snake_splat_eggjuice.tga
		BhvrOverRide
			FadeInLength		30
			FadeOutLength		30
			Scale			10 10 10
			#SplatFlags		MULTIPLY
			SplatSetBack		.5    #0 -> 1.0 splatSetBack = what percent of the splat should be behind the node, and what in front
			SplatFalloffType	NONE			# UP DOWN NONE BOTH	
			SplatFadeCenter		1			# only for UP and DOWN falloff
			Alpha			180
		
			StartColor		255 255 255
		End
	End
End



End	

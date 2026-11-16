#########################################################
##	jmp1_trampoline_sml1
##
FxInfo

Input  
	InpName	Origin
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Clouds
		Type	start 
		At	origin

		Part	:jmp1_tramp_sml_main.part	##Snow##
		Part	:jmp1_fil1.part
		Part	:jmp1_ringglow1.part
		Part	:jmp1_tramp_sml_rings.part
	End

End

Condition
	On 	Time
	Time 	90

	Event
		EName 	Clouds
		Type	start 
		
		Part	:SnowflakeFast_main.part
	End

End

#########################################################################
##Wind Geoms
############################

Condition
	On 	cycle
	Time 	2

	Event
		EName   Wind
		Type	start 
		At	origin
		ChildFx	:WinterEventLauncherWind.fx
		LifeSpan	110
	End
End

##########################################################################
###Splat snow
#############################
#
#Condition
#	On 	cycle
#	Time 	1
#
#	Event
#		EName   Wind
#		Type	start 
#		At	origin
#
#		splat	snow.tga
#		
#		Bhvr	:snowSplatSmall.bhvr
#
#		LifeSpan	410
#	End
#	
#	Event
#		EName   Wind
#		Type	start 
#		At	origin
#
#		splat	snow.tga
#		
#		Bhvr	:snowSplatSmall.bhvr
#
#		LifeSpan	410
#	End
#	
#	Event
#		EName   Wind
#		Type	start 
#		At	origin
#
#		splat	snow.tga
#		
#		Bhvr	:snowSplatSmall.bhvr
#
#		LifeSpan	410
#	End
#	
#	Event
#		EName   Wind
#		Type	start 
#		At	origin
#
#		splat	snow.tga
#		
#		Bhvr	:snowSplatSmall.bhvr
#
#		LifeSpan	410
#	End
#
#	Event
#		EName   Wind
#		Type	start 
#		At	origin
#
#		splat	snow.tga
#		
#		Bhvr	:snowSplatSmall.bhvr
#
#		LifeSpan	410
#	End
#	
#	Event
#		EName   Wind
#		Type	start 
#		At	origin
#
#		splat	snow.tga
#		
#		Bhvr	:snowSplatSmall.bhvr
#
#		LifeSpan	410
#	End
#	
#	Event
#		EName   Wind
#		Type	start 
#		At	origin
#
#		splat	snow.tga
#		
#		Bhvr	:snowSplatSmall.bhvr
#
#		LifeSpan	410
#	End
#	
#	Event
#		EName   Wind
#		Type	start 
#		At	origin
#
#		splat	snow.tga
#		
#		Bhvr	:snowSplatSmall.bhvr
#
#		LifeSpan	410
#	End
#
#	Event
#		EName   Wind
#		Type	start 
#		At	origin
#
#		splat	snow.tga
#		
#		Bhvr	:snowSplatSmall.bhvr
#
#		LifeSpan	410
#	End
#	
#	Event
#		EName   Wind
#		Type	start 
#		At	origin
#
#		splat	snow.tga
#		
#		Bhvr	:snowSplatSmall.bhvr
#
#		LifeSpan	410
#	End
#	
#	Event
#		EName   Wind
#		Type	start 
#		At	origin
#
#		splat	snow.tga
#		
#		Bhvr	:snowSplatSmall.bhvr
#
#		LifeSpan	410
#	End
#	
#	Event
#		EName   Wind
#		Type	start 
#		At	origin
#
#		splat	snow.tga
#		
#		Bhvr	:snowSplatSmall.bhvr
#
#		LifeSpan	410
#	End
#
#End

End	


			
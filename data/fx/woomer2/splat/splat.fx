#########################################################
##	pump_smoke
##
FxInfo

Condition
	Event
		Type	local
		At		Root 
		
		bhvroverride
			Scale			1.0  50  1.0		### Y= Height of Cylinder from center
			Rgb0			255 255 255
			SplatFlags			ADDITIVE
			SplatSetBack		0.5     #0 -> 1.0 splatSetBack = what percent of the splat should be behind the node, and what in front
			SplatFalloffType	BOTH			# UP DOWN NONE BOTH	
			SplatFadeCenter		1.0				# only for UP and DOWN falloff
		end
		#Splat	Generic_ring   
		#Splat	mistring01	woomersphere2
		Splat	woomersphere2 woomersphere2 
	End 		
	Event
		Type	Local
		At		Root            
		Geom Fx_IceWeapon
	End
End



End			
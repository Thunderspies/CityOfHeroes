#########################################################
##	RadiationAura
FxInfo

Input  
	InpName	root
End


##################################

Condition
	On	Time
	Time	15
	
	Event
		Type	Local
		At	root
		
		Part	:SnowFall_Light_Ground.part
		Part	:SnowFall_Light_Glow_Ground.part
		Part	:SnowFall_Light2_Ground.part
		Part	:SnowFall_Light3_Ground.part
			
	End

End

Condition
	On	Time
	Time	0
	
	Event
		EName	Hookup
		Type	Local
		At	root
		
	End

	Event
		Type	local
		At	Hookup
		
		Part	:SnowFall_Light.part
		Part	:SnowFall_Light_Glow.part
		Part	:SnowFall_Light2.part
		Part	:SnowFall_Light3.part
		Part	:SnowFall_Light.part
		Part	:SnowFall_Light_Glow.part
		Part	:SnowFall_Light2.part
		Part	:SnowFall_Light3.part
#		Part	:SnowFall_Light.part
#		Part	:SnowFall_Light_Glow.part
#		Part	:SnowFall_Light2.part
#		Part	:SnowFall_Light3.part
#		Part	:SnowFall_Light.part
#		Part	:SnowFall_Light_Glow.part
#		Part	:SnowFall_Light2.part
#		Part	:SnowFall_Light3.part
		
	End
		Event
		EName	Hookup2
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 5 30
		End
		#Sound	blizzard2 100 30 .9
		
	End

	Event
		Type	local
		At	Hookup2
		
		Part	:SnowSpeed_Light.part
		Part	:SnowSpeed_Light_Glow.part
		Part	:SnowSpeed_Light2.part
		Part	:SnowSpeed_Light3.part
		Part	:SnowSpeed_Light.part
		Part	:SnowSpeed_Light_Glow.part
		Part	:SnowSpeed_Light2.part
		Part	:SnowSpeed_Light3.part
#		Part	:SnowSpeed_Light.part
#		Part	:SnowSpeed_Light_Glow.part
#		Part	:SnowSpeed_Light2.part
#		Part	:SnowSpeed_Light3.part
#		Part	:SnowSpeed_Light.part
#		Part	:SnowSpeed_Light_Glow.part
#		Part	:SnowSpeed_Light2.part
#		Part	:SnowSpeed_Light3.part
		
	End
	
End

End		
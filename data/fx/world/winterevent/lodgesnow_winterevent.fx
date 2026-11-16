#########################################################
##	RadiationAura
FxInfo

Input  
	InpName	root
End


##################################

Condition
	On	Time
	Time	155
	
	Event
		Type	Local
		At	root
		
		BhvrOverride
			PositionOffset	0 -30 0
		End

		Part	:SnowFall_Ground.part
		Part	:SnowFall_Glow_Ground.part
		Part	:SnowFall2_Ground.part
		Part	:SnowFall3_Ground.part
			
	End

End

Condition
	On	Time
	Time	0
	
	Event
		Type	local
		At	root
		
		Part	:SnowFall.part
		Part	:SnowFall_Glow.part
		Part	:SnowFall2.part
		Part	:SnowFall3.part
		Part	:SnowFall.part
		Part	:SnowFall_Glow.part
		Part	:SnowFall2.part
		Part	:SnowFall3.part
		Part	:SnowFall.part
		Part	:SnowFall_Glow.part
		Part	:SnowFall2.part
		Part	:SnowFall3.part
		Part	:SnowFall.part
		Part	:SnowFall_Glow.part
		Part	:SnowFall2.part
		Part	:SnowFall3.part
		
	End

	Event
		Type	local
		At	root
		
		Part	:SnowSpeed.part
		Part	:SnowSpeed_Glow.part
		Part	:SnowSpeed2.part
		Part	:SnowSpeed3.part
		Part	:SnowSpeed.part
		Part	:SnowSpeed.part
		Part	:SnowSpeed2.part
		Part	:SnowSpeed3.part
		Part	:SnowSpeed.part
		Part	:SnowSpeed_Glow.part
		Part	:SnowSpeed2.part
		Part	:SnowSpeed3.part
		Part	:SnowSpeed.part
		Part	:SnowSpeed_Glow.part
		Part	:SnowSpeed2.part
		Part	:SnowSpeed3.part
		
	End
	
End

End		
#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	root
End

Input  
	Inpname	mystic
End

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

##############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	p
		Type	local	#start
		At	WepR
		Part1	:V_Mako_SpinTrails.part  #ClawStreak.part
		Part1	:V_Mako_SpinTrails.part
		Part1	:V_Mako_SpinTrails.part
		Part1	:V_Mako_SpinTrails.part
		Sound Clawspin 100 100 1.0
	End

End

#Condition
#	On 	Time
#	Time 	34
#
#	Event
#		EName	Streakburst
#		Type	local	#start
#		At	WepR
#		part1	:ClawEmbers.part
#		part2	:ClawEmbersBroad.part
#		lifespan 6
#	End
#
#End
Condition
	On 	Time
	Time 	0

	Event
		EName	startworkaround ##start isnt working at all for bones...it does the start of the animation not it's position at this time
		Type	positonly	#start
		At	mystic 
		lookat	t_root
	End

End
Condition
	On 	Time
	Time 	34

	Event
#		EName	clawstreaks
		Type	local	#start
		At	startworkaround
		part1	:ClawEmbers_mako.part
		bhvroverride
			positionoffset	0 0.2 0
			InitialVelocity	0 -.1 1
		end
		lifespan 8
	End
	Event
		Type	local
		At	startworkaround
		part1	:ClawEmbers_mako.part
		bhvroverride
			positionoffset	.3 0.2 0
			InitialVelocity	.15 -.1 .9
		end
		lifespan 8
	End
	Event
		Type	local
		At	startworkaround
		part1	:ClawEmbers_mako.part
		bhvroverride
			positionoffset	-.3 0.2 0
			InitialVelocity	-.15 -.1 .90
		end
		lifespan 8
	End

End

Condition
	On 	Time
	Time 	35

	Event
		EName	Ring
		Type	local #start
		At	startworkaround		
		bhvr	:V_Mako_SpinGrow.bhvr
		bhvroverride
			InitialVelocity 0 -.15 0
		end
	End

End

Condition
	On 	cycle
	Time 	1

	Event
		EName	Ring
		Type	start
		At	Ring #startworkaround
		Part5	:V_Mako_Spinwave.part
		Part5	:V_Mako_SpinSplashes.part
		lifespan 1

	End

End


Condition
	On Time 
	Time 50
	
	Event
		EName	Streakburst
		Type 	Destroy
				
	End

	Event
		EName	Ring
		Type 	Destroy
				
	End

End


Condition
	On Time 
	Time 38
	Event
		EName	p
		Type 	Destroy
				
	End
End


Condition
	On Time 
	Time 75
	
	Event
		EName	all
		Type 	Destroy
				
	End

End

End
##################################


#########################################################
##	Appendage Flares
##

FxInfo


############################################
############## Glows Arm R #################
############################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Glow_UArmR
		Type	Local 
		At	UArmR
		Part1	POWERS\PowerPunch\Largepowerpunch1.part
		Part2	POWERS\PowerPunch\Largepowerpunch2.part
		Part3   POWERS\PowerPunch\LargepowerpunchGlow.part
		Part4   POWERS\PowerPunch\powerpunchRings.part
		Part5	POWERS\PowerPunch\Largepowerpunch3.part
		#Sound	PPfocus2 100 30 .7
		Lifespan 31

	End

		Event
		EName 	Glow_ColR
		Type	Local 
		At	Col_R
		bhvr	behaviors\FX_RS_GlowColR.bhvr
		Part1	POWERS\PowerPunch\Largepowerpunch1.part
		Part2	POWERS\PowerPunch\Largepowerpunch2.part
		Part3   POWERS\PowerPunch\LargepowerpunchGlow.part
		Part4   POWERS\PowerPunch\powerpunchRings.part
		Part5	POWERS\PowerPunch\Largepowerpunch3.part
		#Sound	PPfocus2 100 30 .7
		Lifespan 31

	End

End


############################################
############## Glows Arm L #################
############################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Glow_UArmL
		Type	Local 
		At	UArmL
		Part1	POWERS\PowerPunch\Largepowerpunch1.part
		Part2	POWERS\PowerPunch\Largepowerpunch2.part
		Part3   POWERS\PowerPunch\LargepowerpunchGlow.part
		Part4   POWERS\PowerPunch\powerpunchRings.part
		Part5	POWERS\PowerPunch\Largepowerpunch3.part
		#Sound	PPfocus2 100 30 .7
		Lifespan 46

	End

		Event
		EName 	Glow_ColL
		Type	Local 
		At	Col_L
		bhvr	behaviors\FX_RS_GlowColL.bhvr
		Part1	POWERS\PowerPunch\Largepowerpunch1.part
		Part2	POWERS\PowerPunch\Largepowerpunch2.part
		Part3   POWERS\PowerPunch\LargepowerpunchGlow.part
		Part4   POWERS\PowerPunch\powerpunchRings.part
		Part5	POWERS\PowerPunch\Largepowerpunch3.part
		#Sound	PPfocus2 100 30 .7
		Lifespan 46

	End

End


############################################
############## Glows Hip L #################
############################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Glow_UArmL
		Type	Local 
		At	ULegL
		Part1	POWERS\PowerPunch\Largepowerpunch1.part
		Part2	POWERS\PowerPunch\Largepowerpunch2.part
		Part3   POWERS\PowerPunch\LargepowerpunchGlow.part
		Part4   POWERS\PowerPunch\powerpunchRings.part
		Part5	POWERS\PowerPunch\Largepowerpunch3.part
		#Sound	PPfocus2 100 30 .7
		Lifespan 37

	End

		Event
		EName 	Glow_ColL
		Type	Local 
		At	Hips
		bhvr	behaviors\FX_RS_GlowHipL.bhvr
		Part1	POWERS\PowerPunch\Largepowerpunch1.part
		Part2	POWERS\PowerPunch\Largepowerpunch2.part
		Part3   POWERS\PowerPunch\LargepowerpunchGlow.part
		Part4   POWERS\PowerPunch\powerpunchRings.part
		Part5	POWERS\PowerPunch\Largepowerpunch3.part
		#Sound	PPfocus2 100 30 .7
		Lifespan 37

	End

End


############################################
############## Glows Hip R #################
############################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Glow_UArmR
		Type	Local 
		At	ULegR
		Part1	POWERS\PowerPunch\Largepowerpunch1.part
		Part2	POWERS\PowerPunch\Largepowerpunch2.part
		Part3   POWERS\PowerPunch\LargepowerpunchGlow.part
		Part4   POWERS\PowerPunch\powerpunchRings.part
		Part5	POWERS\PowerPunch\Largepowerpunch3.part
		#Sound	PPfocus2 100 30 .7
		Lifespan 55

	End

		Event
		EName 	Glow_ColR
		Type	Local 
		At	Hips
		bhvr	behaviors\FX_RS_GlowHipR.bhvr
		Part1	POWERS\PowerPunch\Largepowerpunch1.part
		Part2	POWERS\PowerPunch\Largepowerpunch2.part
		Part3   POWERS\PowerPunch\LargepowerpunchGlow.part
		Part4   POWERS\PowerPunch\powerpunchRings.part
		Part5	POWERS\PowerPunch\Largepowerpunch3.part
		#Sound	PPfocus2 100 30 .7
		Lifespan 55

	End

End


#######################################################################################
########################################################## Rings Arms #################
#######################################################################################

Condition
	On 	Time
	Time 	32

	Event
		EName 	Ring_UArmR
		Type	Local 
		At	UArmR
		
		Part3   POWERS\PowerPunch\QuickRings.part

	End
End

Condition
	On 	Time
	Time 	46

	Event
		EName 	Ring_UArmL
		Type	Local 
		At	UArmL
		
		Part3   POWERS\PowerPunch\QuickRings.part

	End

End


############################################
############## Rings Legs ##################
############################################


Condition
	On 	Time
	Time 	38

	Event
		EName 	Ring_HipL
		Type	Local 
		At	ULegL
		
		Part3   POWERS\PowerPunch\QuickRings.part

	End

End

Condition
	On 	Time
	Time 	54

	Event
		EName 	Ring_HipR
		Type	Local 
		At	ULegR
		
		Part3   POWERS\PowerPunch\QuickRings.part

	End
End


############################################
############## Kills #######################
############################################

Condition
	On 	Time
	Time 	200

	Event
		EName 	all
		Type	Destroy

	End

End


End		
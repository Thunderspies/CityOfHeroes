#########################################################
##	Predefined arc curve
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	HandR
End

LifeSpan	30

########################################################

Condition

	On 	time
	Time 	0

	Event
		Ename	hitspot1
		Type	posit
	
		At	chest
#		geom    Testing_Target
		lookat t_waist
	End


#############


#
#	Event
#		Ename	hitsplash1
#		Type	local
#		at	hips
#
#		Part	:Bot_Blaster_HitBigfire.part
#		lifespan 110
#	end


	Event
		Type	local
	
		At	hitspot1
		bhvr	:Bot_Blaster_ShotHitRot.bhvr
		Part	:Bot_Blaster_ShotHitSmoulder.Part
		Part	:Bot_Blaster_ShotHitSparks.Part
		Part	:Bot_Blaster_ShotHitSparksBlue.Part
		Sound Arachnosbot_blasthit 70 70 .6
		lifespan 90
	End
	
	Event
		Ename	hitsplash1
		Type	local
	
		At	hitspot1
		Part	:Bot_Blaster_ShotHitFlame.Part
		Part	:Bot_Blaster_ShotHitFlame.Part
#		Part	:Bot_Blaster_ShotHitFlame.Part
#		Part	:Bot_Blaster_ShotHitFlame.Part
#		Part	:Bot_Blaster_ShotHitFlame.Part
		pmagnet up
		lifespan 70
	End

end

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

LifeSpan	90

########################################################

Condition

	On 	time
	Time 	0

	Event
		Ename	hitspot1
		Type	posit
	
		At	chest
#		geom    Testing_Target
		bhvr	:Bot_Blaster_BurstHit.bhvr
		Sound Arachnosbot_blasthit 80 80 .88
		lookat t_waist
	End


#############


	Event
		Ename	up
		Type	positonly
		at	root
		bhvr	up.bhvr
	end

	Event
		Ename	hitsplash1
		Type	local
		at	hips

		Part	:Bot_Blaster_HitBigfire.part
		lifespan 110
	end
	Event
		Ename	hitsplash1
		Type	local
	
		At	hitspot1
		bhvr	:Bot_Blaster_HitRot.bhvr
		Part	:Bot_Blaster_HitSparks.Part
		Part	:Bot_Blaster_HitSmoulder.Part
		lifespan 90
	End
	
	Event
		Ename	hitsplash1
		Type	local
	
		At	hitspot1
		Part	:Bot_Blaster_HitFlame.Part
		Part	:Bot_Blaster_HitFlame.Part
		Part	:Bot_Blaster_HitFlame.Part
		Part	:Bot_Blaster_HitFlame.Part
		Part	:Bot_Blaster_HitFlame.Part
		pmagnet up
		lifespan 70
	End

end


Condition

	On 	time
	Time 	4

	Event
		Ename	hitspot2
		Type	posit
	
		At	Col_L #head
		bhvr	:Bot_Blaster_BurstHit.bhvr
#		geom    Testing_Target
		lookat t_waist
	End


#############


	Event
		Ename	up
		Type	positonly
		at	root
		bhvr	up.bhvr
	end

	Event
		Ename	hitsplash2
		Type	local
	
		At	hitspot2
		bhvr	:Bot_Blaster_HitRot.bhvr
#		Part	:Bot_Blaster_HitSparks.Part
		Part	:Bot_Blaster_HitSmoulder.Part
		lifespan 90
	End
	
	Event
		Ename	hitsplash2
		Type	local
	
		At	hitspot2
		Part	:Bot_Blaster_HitFlame.Part
		Part	:Bot_Blaster_HitFlame.Part
		Part	:Bot_Blaster_HitFlame.Part
		Part	:Bot_Blaster_HitFlame.Part
		Part	:Bot_Blaster_HitFlame.Part
		pmagnet up
		lifespan 70
	End

end


Condition

	On 	time
	Time 	8

	Event
		Ename	hitspot3
		Type	posit
	
		At	Col_R #hips
		bhvr	:Bot_Blaster_BurstHit.bhvr
#		geom    Testing_Target
		lookat t_waist
	End


#############


	Event
		Ename	up
		Type	positonly
		at	root
		bhvr	up.bhvr
	end

	Event
		Ename	hitsplash3
		Type	local
	
		At	hitspot3
		bhvr	:Bot_Blaster_HitRot.bhvr
		Part	:Bot_Blaster_HitSparks.Part
		Part	:Bot_Blaster_HitSmoulder.Part
		lifespan 90
	End
	
	Event
		Ename	hitsplash3
		Type	local
	
		At	hitspot3
		Part	:Bot_Blaster_HitFlame.Part
		Part	:Bot_Blaster_HitFlame.Part
		Part	:Bot_Blaster_HitFlame.Part
		Part	:Bot_Blaster_HitFlame.Part
		Part	:Bot_Blaster_HitFlame.Part
		pmagnet up
		lifespan 70
	End

end


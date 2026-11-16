#########################################################
##	General red electricity crawling across characters limbs
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

LifeSpan	60

########################################################


Condition

	On 	time
	Time 	0

	Event
		Ename	lift
		Type	local
	
		At	root
		bhvr :mu_StaticSpot.bhvr
		Sound StaticDischarge2 90 90 .9
		lifespan 200
		#geom    Testing_Target
	End

	Event
		Type	start  
		At	lift
		childfx	:mu_StaticArc_090.fx
		lifespan 50
	end

	Event
		Type	start  
		At	lift
		childfx	:mu_StaticArc_090.fx
		lifespan 50
	end

	Event
		Type	start  
		At	lift
		childfx	:mu_StaticArc_090.fx
		lifespan 50
	end


end

Condition

	On 	time
	Time 	0
event
		Type	positonly
		At	Hips
		Sound eleccontrol2 100 100 .5
end
end

Condition

	On 	cycle
	Time 	7

	Event
		Type	start  
		At	lift
		childfx	:mu_StaticArc_090.fx
		lifespan 50
	end

end
Condition

	On 	cycle
	Time 	3

	Event
		Type	start  
		At	lift
		childfx	:mu_StaticArc_090.fx
		lifespan 50
	end

end
Condition

	On 	cycle
	Time 	5

	Event
		Type	start  
		At	lift
		childfx	:mu_StaticArc_090.fx
		lifespan 50
	end

end
#######################################

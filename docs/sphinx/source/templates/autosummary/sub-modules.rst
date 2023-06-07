..
  sub-module.rst

{{ fullname | escape | underline }}

General description
-------------------

.. automodule:: {{ fullname }}


API Table of Content
--------------------

{% block modules %}
    {% if modules %}
    .. rubric:: Sub-modules
    .. autosummary::
        :toctree: {{ fullname }}/submodules
        {% for item in modules %}
        {{ item }}
        {%- endfor %}
    {% endif %}
{% endblock %}
{% block attributes %}
    {% if attributes %}
        .. rubric:: Module Attributes
        .. autosummary::
            :toctree: {{ fullname }}/attributes
            {% for item in attributes %}
            {{ item }}
            {%- endfor %}
    {% endif %}
{% endblock %}
{% block functions %}
    {% if functions %}
        .. rubric:: {{ _('Functions') }}
        .. autosummary::
            :toctree: {{ fullname }}/functions
            :nosignatures:
            :template: autosummary/base.rst
            {% for item in functions %}
            {{ item }}
            {%- endfor %}
    {% endif %}
{% endblock %}
{% block classes %}
    {% if classes %}
        .. rubric:: {{ _('Classes') }}
        .. autosummary::
            :toctree: {{ fullname }}/classes
            :nosignatures:
            :template: autosummary/class.rst
            :recursive:
            {% for item in classes %}
            {{ item }}
            {%- endfor %}
    {% endif %}
{% endblock %}
{% block exceptions %}
    {% if exceptions %}
        .. rubric:: {{ _('Exceptions') }}
        .. autosummary::
            :toctree: {{ fullname }}/exceptions
            :nosignatures:
            :template: base.rst
            {% for item in exceptions %}
            {{ item }}
        {%- endfor %}
    {% endif %}
{% endblock %}




